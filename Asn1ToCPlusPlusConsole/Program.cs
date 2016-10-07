using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace Asn1ToCPlusPlusConsole
{
  class IdHelper
  {
    public static string Id(string identifier)
    {
      if ("true" == identifier) return "true_";
      if ("false" == identifier) return "false_";
      if ("new" == identifier) return "new_";
      if ("class" == identifier) return "class_";
      if ("if" == identifier) return "if_";
      // Replace "-" with "﹇"   Other options: ‿ ⁀ ⁔ ﹂ ﹃ ﹄ ﹉
      // var rval = identifier.Replace("-", "﹇"); // .Replace("_", "﹇_");
      var rval = identifier.Replace("-", "_"); // .Replace("_", "﹇_");
      return rval;
    }
  }

  public interface IAsn1TypeList
  {
    void AddType(Asn1Type type);
  }

  public class Context
  {
    public Context()
    {
      // Identifiers = new List<Asn1TypedIdentifier>();
      TypesByModule = new Dictionary<string, List<Asn1Type>>();
      ImportsByModule = new Dictionary<string, List<Asn1ToCPlusPlusConsole.Imports>>();
    }

    // public List<Asn1TypedIdentifier> Identifiers { get; }
    public List<Asn1Type> Types { get { return TypesByModule[moduleName_]; } }
    public Dictionary<string, List<Asn1Type>> TypesByModule;
    private string moduleName_;
    public List<Imports> Imports { get { return ImportsByModule[moduleName_]; } }
    public Dictionary<string,List<Imports>> ImportsByModule;

    public void SetModule(string moduleName)
    {
      moduleName_ = moduleName;
      if (!TypesByModule.ContainsKey(moduleName_))
      {
        TypesByModule[moduleName_] = new List<Asn1Type>();
        ImportsByModule[moduleName_] = new List<Imports>();
      }
    }

    public string GetModuleForType(string typeName)
    {
      if (TypesByModule[moduleName_].Any(type => type.Name == typeName ))
      {
        return moduleName_;
      }
      foreach (var moduleName in TypesByModule.Keys)
      {
        if (TypesByModule[moduleName].Any(type => type.Name == typeName))
        {
          return moduleName;
        }
      }
      foreach (var moduleName in ImportsByModule.Keys)
      {
        foreach (var imports in ImportsByModule[moduleName])
        {
          foreach (var pair in imports.symbolsFromModules_)
          {
            if (pair.Item2.Contains(typeName))
            {
              return pair.Item1;
            }
          }
        }
      }
      return null;
    }
  }

  public class Definitions
  {

    public Definitions()
    {
    }


    public string ModuleName { get; internal set; }

    public void Read(Parser parser, Context context)
    {
      // CPDLCAPDUsVersion1 DEFINITIONS ::= BEGIN ... END
      ModuleName = parser.NextToken();
      context.SetModule(ModuleName);
      parser.Expect("DEFINITIONS");
      parser.Expect("::=");
      parser.Expect("BEGIN");
      string keyword = parser.NextToken();
      while (keyword != "END")
      {
        if (keyword.Equals("IMPORTS"))
        {
          var imports = new Imports();
          imports.Read(parser);
          context.Imports.Add(imports);
          keyword = parser.NextToken();
        }
        else {
          parser.Expect("::=");
          var nextToken = parser.NextToken();
          if (nextToken.Equals("CHOICE"))
          {
            Choice choice = new Choice(keyword, ModuleName);
            choice.Read(parser, context);
            context.Types.Add(choice);
            // context.Identifiers.Add(new Asn1TypedIdentifier(keyword, choice));
            // typesUsed_.UnionWith(choice.getUsedTypes());
          }
          else if (nextToken.Equals("SEQUENCE"))
          {
            if ("{" == parser.PeekToken())
            {
              Sequence seq = new Sequence(keyword, ModuleName);
              seq.Read(parser, context);
              context.Types.Add(seq);
            }
            else
            {
              // Assume it's a SequenceOf
              SequenceOf seqOf = new SequenceOf(keyword, ModuleName);
              seqOf.Read(parser, context);
              context.Types.Add(seqOf);
            }
            // typesUsed_.UnionWith(seq.getUsedTypes());
          }
          else if (nextToken.Equals("ENUMERATED")) {
            Enumerated enumerated = new Enumerated(keyword, ModuleName);
            enumerated.Read(parser);
            context.Types.Add(enumerated);
          }
          else if (nextToken.Equals("RELATIVE-OID")) {
            RelativeOid oid = new RelativeOid(keyword ,ModuleName);
            context.Types.Add(oid);
          }
          else if (nextToken.Equals("BIT")) // BIT STRING
          {
            parser.Expect("STRING");
            var range = new Range();
            if (parser.PeekToken("("))
            {
              range.Read(parser);
            }
            BitString bitString = new BitString(keyword, ModuleName);
            bitString.Read(parser);
            context.Types.Add(bitString);
          }
          else if (nextToken.Equals("INTEGER"))
          {
            var type = new Long(keyword, ModuleName);
            type.Read(parser);
            context.Types.Add(type);
          }
          else if (nextToken.Equals("IA5String"))
          {
            var type = new IA5String(keyword, ModuleName);
            type.Read(parser);
            context.Types.Add(type);
          }
          else if (nextToken.Equals("NumericString"))
          {
            var type = new NumericString(keyword, ModuleName);
            type.Read(parser);
            context.Types.Add(type);
          }
          else
          {
            // Assume this is a typedef
            var type = new Typedef(keyword, ModuleName, nextToken);
            // type.Read(parser);
            context.Types.Add(type);
          }
          keyword = parser.NextToken();
        }
      }
    }

    public void WriteDefinitions(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      hWriter.WriteLine("// {0}.h", ModuleName);
      hWriter.WriteLine("//");
      hWriter.WriteLine("#pragma once");
      hWriter.WriteLine();
      hWriter.WriteLine("#include <iterator>");
      hWriter.WriteLine("#include <vector>");
      hWriter.WriteLine("#include \"Asn1.h\"");
      hWriter.WriteLine();

      cxxWriter.WriteLine("// {0}.cxx", ModuleName);
      cxxWriter.WriteLine("//");
      cxxWriter.WriteLine();
      cxxWriter.WriteLine("#include \"{0}.h\"", ModuleName);
      cxxWriter.WriteLine();

      foreach (var import in context.ImportsByModule[ModuleName])
      {
        import.Write(cxxWriter, hWriter);
      }
      if (context.ImportsByModule[ModuleName].Count > 0)
      {
        hWriter.WriteLine();
        cxxWriter.WriteLine();
      }

      List<Asn1Type> typesDefined = context.TypesByModule[ModuleName];

      hWriter.WriteLine("namespace {0} {{", ModuleName);
      cxxWriter.WriteLine("namespace {0} {{", ModuleName);
      foreach (var type in typesDefined)
      {
        if (type.IsTypedef() && type.IsBasic())
        {
          type.Write(cxxWriter, hWriter);
        }
      }
      foreach (var type in typesDefined)
      {
        if (type.IsTypedef())
        {
          continue;
        }
        hWriter.WriteLine("class {0};", type.Name);
      }
      foreach (var type in typesDefined)
      {
        if (type.IsTypedef() && !type.IsBasic())
        {
          type.Write(cxxWriter, hWriter);
        }
      }
      if (typesDefined.Count > 0) hWriter.WriteLine();

      foreach (var type in typesDefined)
      {
        if (!type.IsTypedef())
        {
          type.Write(cxxWriter, hWriter);
        }
      }
      hWriter.WriteLine("}");
      cxxWriter.WriteLine("}");
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      hWriter.WriteLine("{0}Serializer.h", ModuleName);
      hWriter.WriteLine("//");
      hWriter.WriteLine("#pragma once");
      hWriter.WriteLine();
      hWriter.WriteLine("# include <iterator>");
      hWriter.WriteLine("# include <vector>");
      hWriter.WriteLine("# include \"Asn1.h\"");
      hWriter.WriteLine();
      hWriter.WriteLine("# include \"{0}.h\"", ModuleName);
      hWriter.WriteLine("# include \"Asn1PerSerializer.h\"");
      hWriter.WriteLine();
      hWriter.WriteLine("namespace {0} {{", ModuleName);
      hWriter.WriteLine();
      hWriter.WriteLine("class Serializer : public PerSerializer::Serializer {");
      hWriter.WriteLine("public:");
      hWriter.WriteLine("  PerSerializer::BitStream stream;");
      hWriter.WriteLine("};");

      List<Asn1Type> typesDefined = context.TypesByModule[ModuleName];
      foreach (var type in typesDefined)
      {
        if (!type.IsTypedef())
        {
          type.WriteSerializers(cxxWriter, hWriter, context);
        }
      }

      hWriter.WriteLine("} // namespace");
    }
};

  public class NumericString : Asn1Type
  {
    private string identifier_;
    private Range range_;

    string Asn1Type.Name { get { return string.Format("Asn1::Asn1String<{0}, {1}>", range_.Begin, range_.End); } }

    public string ModuleName { get; private set; }

    public NumericString(string identifier, string moduleName)
    {
      identifier_ = identifier;
      ModuleName = moduleName;
    }

    public void Read(Parser parser)
    {
      range_ = new Range();
      range_.Read(parser);
    }

    public bool IsTypedef()
    {
      return true;
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("typedef Asn1::Asn1String<{0}, {1}> {2}; // NumericString", range_.Begin, range_.End, identifier_);
      hWriter.Flush();
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }


  public class Typedef : Asn1Type
  {
    public string ModuleName { get; internal set; }
    private string typeName_;
    private string alias_;

    //public Typedef(string typeName, string moduleName)
    //{
    //  typeName_ = typeName;
    //  ModuleName = moduleName;
    //}

    public Typedef(string typeName, string moduleName, string alias) //: this(typeName, moduleName)
    {
      typeName_ = typeName;
      ModuleName = moduleName;
      alias_ = alias;
    }

    public string Name
    {
      get
      {
        return typeName_;
      }
    }

    public bool IsTypedef()
    {
      return true;
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("typedef {0} {1};", alias_, typeName_);
      hWriter.Flush();
    }

    public void Read(Parser parser)
    {
      alias_ = parser.NextToken();
    }

    public bool IsBasic()
    {
      return false;
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class IA5String : Asn1Type
  {
    public string ModuleName { get; internal set; }
    string Asn1Type.Name { get { return string.Format("Asn1::Asn1IA5String<{0}, {1}>", range_.Begin, range_.End); } }
    private string identifier_;
    private Range range_;

    public IA5String(string identifier, string moduleName)
    {
      identifier_ = identifier;
      ModuleName = moduleName;
    }

    public void Read(Parser parser)
    {
      range_ = new Range();
      range_.Read(parser);
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      // hWriter.WriteLine("typedef Asn1::Asn1IA5String<{0}, {1}> {2};", range_.Begin, range_.End, identifier_);
      hWriter.WriteLine("class {0} : public Asn1::Asn1IA5String<{1}, {2}> {{", identifier_, range_.Begin, range_.End);
      hWriter.WriteLine("public:");
      hWriter.WriteLine("  {0}() : Asn1IA5String() {{ }}", identifier_);
      hWriter.WriteLine("  {0}(const {0}&src) : Asn1IA5String(src) {{ }}", identifier_);
      hWriter.WriteLine("};");
    }

    public bool IsTypedef()
    {
      return true;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class Long : Asn1Type
  {
    public string ModuleName { get; internal set; }
    string Asn1Type.Name { get { return string.Format("Asn1::Asn1Int<{0}, {1}>", range_.Begin, range_.End); } }
    private string identifier_;
    private Range range_;

    public Long(string identifier, string moduleName)
    {
      identifier_ = identifier;
      ModuleName = moduleName;
    }

    public void Read(Parser parser)
    {
      range_ = new Range();
      range_.Read(parser);
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      // typedef Asn1::Asn1Int < -600, 70000 > LevelFeet;    // unit = Feet, Range (-600..70000), resolution = 10
      // hWriter.WriteLine("typedef Asn1::Asn1Int<-600, 70000> LevelFeet;    // unit = Feet, Range (-600..70000), resolution = 10");
      hWriter.WriteLine("typedef Asn1::Asn1Int<{0}, {1}> {2};", range_.Begin, range_.End, identifier_);
      hWriter.Flush();
    }

    public bool IsTypedef()
    {
      return true;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class Imports
  {
    public List<Tuple<string, List<string>>> symbolsFromModules_ = new List<Tuple<string, List<string>>>();

    public Imports()
    {
    }

    public void Read(Parser parser)
    {
      while (! parser.PeekToken(";"))
      {
        var symbols = new List<string>();
        symbols.Add(parser.NextToken());
        while (parser.PeekToken(","))
        {
          parser.NextToken();
          var symbol = parser.NextToken();
          symbols.Add(symbol);
        }
        parser.Expect("FROM");
        var module = parser.NextToken();
        Tuple<string, List<string>> symbolsFromModule = new Tuple<string, List<string>>(module, symbols);
        symbolsFromModules_.Add(symbolsFromModule);
      }
      parser.NextToken();

    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      foreach (var module in symbolsFromModules_)
      {
        cxxWriter.WriteLine("# include \"{0}.h\"", module.Item1);
        foreach (var import in module.Item2)
        {
          hWriter.WriteLine("namespace {0} {{ class {1}; }}", module.Item1, import);
        }
      }
    }
  }

  public class Range
  {
    private int rangeBegin_, rangeEnd_;
    private int sizeMin_, sizeMax_;
    private bool hasSize_;

    public Range()
    {
    }

    public int Begin { get { return hasSize_ ? sizeMin_ : rangeBegin_; } }
    public int End { get { return hasSize_ ? sizeMax_ : rangeEnd_; } }

    public void Read(Parser parser)
    {
      if (! parser.HasToken("(")) return;

      if (parser.HasToken("SIZE"))
      {
        hasSize_ = true;
        parser.Expect("(");
        sizeMin_ = int.Parse(parser.NextToken());
        if (parser.HasToken(".."))
        {
          sizeMax_ = int.Parse(parser.NextToken());
        }
        else
        {
          sizeMax_ = sizeMin_;
        }
        parser.Expect(")");
      }
      else
      {
        rangeBegin_ = int.Parse(parser.NextToken());
        parser.Expect("..");
        rangeEnd_ = int.Parse(parser.NextToken());
      }
      parser.Expect(")");
    }

    private void ExpectToken(string expected, string token)
    {
      if (token != expected) throw new Exception(string.Format("Invalid definition format. Expected {1} keyword, found {2}", expected, token));
    }
  }

  public class TypeInfo
  {
    public string ModuleName { get; private set; }
    public string TypeName { get; private set; }
    private int tag_;
    private string defaultValue_;
    private Asn1Type innerType_;
    private int innerCount_;

    public Asn1Type InnerType { get { return innerType_; } }

    public TypeInfo(string moduleName)
    {
      ModuleName = moduleName;
      tag_ = 0;
    }

    public int Tag
    {
      get { return tag_; }
    }


    public string DefaultValue
    {
      get { return defaultValue_; }
    }

    public void Read(Parser parser, Context context)
    {
      List<Asn1Type> typesDefined_ = context.TypesByModule[ModuleName];
      if (parser.HasToken("["))
      {
        // ::= PredefinedType ::= PrefixedType ::= TaggedType ::= [tag] Type
        var tag = parser.NextToken();
        tag_ = int.Parse(tag);
        parser.Expect("]");
      }
      TypeName = parser.NextToken();
      if (TypeName == "BIT")
      {
        parser.Expect("STRING");
        TypeName = "Asn1::BitString";
      }
      else if (TypeName == "NULL")
      {
        TypeName = "Asn1::Asn1NULL";
      }
      else if (TypeName == "SEQUENCE")  // "SIZE () OF, OF...
      {
        var peek = parser.PeekToken();
        if ("SIZE" == peek || "OF" == peek)
        {
          TypeName = "SEQUENCE OF";
          // SEQUENCE OF
          SequenceOf seqOf = new SequenceOf("__owner__"+Tag, ModuleName);
          seqOf.Read(parser, context);
          innerType_ = seqOf;
        }
        else
        { // Unnamed inner sequence
          ++innerCount_;
          Sequence seq = new Sequence("InnerSeq_"+innerCount_, ModuleName);
          seq.Read(parser, context);
          innerType_ = seq;
        }
        //var token = NextToken();
        //if (token != "}") throw new Exception(string.Format("Invalid definition format. Expected } keyword, found {1}", token));
      }
      else
      {
        // Check if type is in another module
        string module = context.GetModuleForType(TypeName);
        if (module != null && module != ModuleName)
        {
          ModuleName = module;
        }
      }
      if (parser.HasToken("DEFAULT"))
      {
        defaultValue_ = parser.NextToken();
      }
    }
  }

  public class SequenceOf : Asn1Type
  {
    public string ModuleName { get; private set; }
    private string identifier_;
    private Size size_;
    private TypeInfo innerType_;

    public SequenceOf(string identifier, string moduleName)
    {
      identifier_ = IdHelper.Id(identifier);
      ModuleName = moduleName;
      size_ = new Size();
    }

    public string Name
    {
      get
      {
        return string.Format("Asn1::SequenceOf<{0}, {1}, {2}>", innerType_.TypeName, size_.MinValue, size_.MaxValue);
      }
    }

    public string InnerTypeName
    {
      get
      {
        return innerType_.TypeName;
      }
    }

    public bool IsTypedef()
    {
      return true;
    }

    public void Read(Parser parser, Context context)
    {
      if (parser.HasToken("SIZE"))
      {
        size_.Read(parser);
      }
      parser.Expect("OF");
      innerType_ = new TypeInfo(ModuleName);
      innerType_.Read(parser, context);
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("typedef Asn1::SequenceOf<{0},{2},{3}> {1};", innerType_.TypeName, identifier_, size_.MinValue, size_.MaxValue);
      hWriter.Flush();
    }

    public bool IsBasic()
    {
      return false;
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class Size
  {
    public Size()
    {
    }

    public int MaxValue { get; internal set; }
    public int MinValue { get; internal set; }

    public void Read(Parser parser)
    {
      parser.Expect("(");
      MinValue = int.Parse(parser.NextToken());
      if (parser.HasToken(".."))
      {
        MaxValue = int.Parse(parser.NextToken());
      }
      else
      {
        MaxValue = MinValue;
      }
      parser.Expect(")");
    }
  }

  public class Asn1TypedIdentifier
  {
    public Asn1TypedIdentifier(string identifier, Asn1Type type)
    {
      Identifier = identifier;
      Type = type;
    }
    public string Identifier { get; }
    public Asn1Type Type { get; }
  }

  public interface Asn1Type
  {
    string ModuleName { get; }
    string Name { get; }

    bool IsBasic();
    bool IsTypedef();
    void Write(StreamWriter cxxWriter, StreamWriter hWriter);
    void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context);
  }

  interface ExtensibleType : Asn1Type
  {
    bool isExtensible();
  }

  public class BitString : Asn1Type
  {
    public string ModuleName { get; private set; }
    string Asn1Type.Name { get { return "BitString"; } }

    private string typeName_;
    private Range range_;

    public BitString(string typeName, string moduleName)
    {
      typeName_ = typeName;
      ModuleName = moduleName;
    }

    public void Read(Parser parser)
    {
      range_ = new Range();
      range_.Read(parser);
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      // hWriter.WriteLine("typedef Asn1::BitString {0};", typeName_);
      hWriter.WriteLine("class {0} : public Asn1::BitString {{", typeName_);
      hWriter.WriteLine("public:");
      hWriter.WriteLine("  {0}() : BitString() {{ }}", typeName_);
      hWriter.WriteLine("  {0}(int min, int max) : BitString(min, max) {{ }}", typeName_);
      hWriter.WriteLine("  {0}(const {0}&src) : BitString(src) {{ }}", typeName_);
      hWriter.WriteLine("  const {0}& operator= (const {0}&src) {{", typeName_);
      hWriter.WriteLine("    return dynamic_cast<const {0}&>(BitString::operator=(src));", typeName_);
      hWriter.WriteLine("  }");
      hWriter.WriteLine("  const {0}& operator= (const PrimitiveType v) {{", typeName_);
      hWriter.WriteLine("    return dynamic_cast<const {0}&>(BitString::operator=(v));", typeName_);
      hWriter.WriteLine("  }");
      hWriter.WriteLine("};");
    }

    public bool IsTypedef()
    {
      return true;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class RelativeOid : Asn1Type
  {
    public string ModuleName { get; private set; }
    string Asn1Type.Name { get { return "Asn1::Oid"; } }
    private string typeName_;

    public RelativeOid(string typeName, string moduleName)
    {
      typeName_ = typeName;
      ModuleName = moduleName;
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("typedef Asn1::Oid {0};", typeName_);
    }

    public bool IsTypedef()
    {
      return true;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class Enumerated : ExtensibleType
  {
    public string ModuleName { get; private set; }
    string Asn1Type.Name { get { return typeName_; } }
    private bool extensible_;
    private string typeName_;
    private List<Tuple<string, int>> enumerators_;
    int minValue_, maxValue_;

    public Enumerated(string identifier, string moduleName)
    {
      typeName_ = IdHelper.Id(identifier);
      ModuleName = moduleName;
      enumerators_ = new List<Tuple<string, int>>();
      minValue_ = int.MaxValue;
      maxValue_ = int.MinValue;
    }

    public bool isExtensible()
    {
      return extensible_;
    }

    public void Read(Parser parser)
    {
      parser.Expect("{");
      do
      {
        if (parser.HasToken("..."))
        {
          setExtensible();
        }
        else
        {
          var identifier = parser.NextToken();
          identifier = IdHelper.Id(identifier);
          parser.Expect("(");
          var enumValueToken = parser.NextToken();
          int enumValue = int.Parse(enumValueToken);
          if (minValue_ > enumValue) minValue_ = enumValue;
          if (maxValue_ < enumValue) maxValue_ = enumValue;
          parser.Expect(")");
          enumerators_.Add(new Tuple<string, int>(identifier, enumValue));
        }
      } while (parser.HasToken(","));
      parser.Expect("}");
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("class {0} : public Asn1::Enumerated<{1},{2}> {{", typeName_, minValue_, maxValue_);
      hWriter.WriteLine("public:");
      hWriter.WriteLine("  virtual bool extensible() const {{ return {0}; }}", isExtensible() ? "true" : "false");
      hWriter.WriteLine("  typedef enum {");
      foreach (var enumerated in enumerators_)
      {
        hWriter.WriteLine("    {0} = {1}{2}", enumerated.Item1, enumerated.Item2, (enumerated==enumerators_.Last() ? "" : ","));
      }
      hWriter.WriteLine("  } EnumValues;");
      hWriter.WriteLine("  {0}() : Enumerated() {{ }}", typeName_);
      hWriter.WriteLine("  {0}(int value) : Enumerated(value) {{ }}", typeName_);
      hWriter.WriteLine("};");
      hWriter.WriteLine();
      hWriter.Flush();
    }

    private void setExtensible()
    {
      extensible_ = true;
    }

    public bool IsTypedef()
    {
      return false;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // throw new NotImplementedException();
    }
  }

  public class Sequence : ExtensibleType
  {
    string Asn1Type.Name { get { return typeName_; } }

    public string ModuleName { get; private set; }
    private List<Element> types = new List<Element>();
    private bool extensible_;
    private string typeName_;

    public class Element
    {
      public Element(string moduleName) { typeInfo = new TypeInfo(moduleName); }
      public TypeInfo typeInfo;
      public bool optional;
      public string identifier;
    }

    public void setExtensible() { extensible_ = true; }

    public Sequence(string typeName, string moduleName)
    {
      typeName_ = IdHelper.Id(typeName);
      ModuleName = moduleName;
    }

    public void Read(Parser parser, Context context)
    {
      List<Asn1Type> typesDefined_ = context.TypesByModule[ModuleName];
      parser.Expect("{");
      do
      {
        var identifier = parser.NextToken();
        if (identifier == "...")
        {
          setExtensible();
        }
        else
        {
          Element element = new Element(ModuleName);
          element.identifier = identifier;
          element.typeInfo.Read(parser, context);
          if (parser.PeekToken() == "OPTIONAL")
          {
            parser.NextToken();
            element.optional = true;
          }
          types.Add(element);
        }
      } while (parser.HasToken(","));
      parser.Expect("}");
    }

    public bool isExtensible()
    {
      return extensible_;
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("class {0} : public Asn1::Sequence {{", typeName_);
      hWriter.WriteLine("public:");
      foreach (var item in types)
      {
        //if ("SEQUENCE" == item.typeInfo.TypeName)
        //{
        //  // Inner SEQUENCE
        //  item.Item1.InnerType.Write(cxxWriter, hWriter);
        //  hWriter.WriteLine("  class {0} : public Asn1::Sequence {{", "anon_" + item.Item2.identifier);
        //  hWriter.WriteLine("  public:");

        //}

        // typedef Asn1::SequenceOf < AEQualifierVersionAddress, 1, 256 > SequenceOf_AEQualifierVersionAddress;
        string identifier;
        if ("SEQUENCE OF" == item.typeInfo.TypeName)
        {
          // hWriter.WriteLine("  typedef {1} SequenceOf_{2};", item.typeInfo.InnerType.Name, (item.typeInfo.InnerType as SequenceOf).InnerTypeName);
          identifier = item.typeInfo.InnerType.Name + " ";
        }
        else if ("SEQUENCE" == item.typeInfo.TypeName)
        {
          var savedNewLine = hWriter.NewLine;
          hWriter.NewLine = savedNewLine + "  ";
          hWriter.Write("  ");
          item.typeInfo.InnerType.Write(cxxWriter, hWriter);
          hWriter.NewLine = savedNewLine;
          hWriter.WriteLine();
          // hWriter.WriteLine("  typedef {1} SequenceOf_{2};", item.typeInfo.InnerType.Name, (item.typeInfo.InnerType as SequenceOf).InnerTypeName);
          identifier = item.typeInfo.InnerType.Name + " ";
        }
        else
        {
          if (item.typeInfo.ModuleName != ModuleName)
          {
            identifier = item.typeInfo.ModuleName + "::" + item.typeInfo.TypeName;
          }
          else
          {
            identifier = item.typeInfo.TypeName;
          }
        }

        if (item.optional)
        {
          hWriter.WriteLine("  Optional<{0}>::type {1};", identifier, item.identifier);
        }
        else
        {
          hWriter.WriteLine("  Required<{0}>::type {1};", identifier, item.identifier);
        }
      }
      hWriter.WriteLine("  {0}() : Sequence({1}) {{", typeName_, types.Count());
      if (types.Count() > 0)
      {
        hWriter.WriteLine("    Sequence::Inserter it = back_inserter();");
        hWriter.Write("    it");
      }
      foreach (var item in types)
      {
        hWriter.Write(" << {0}", item.identifier);
      }
      hWriter.WriteLine(";");
      hWriter.WriteLine("  }");
      hWriter.WriteLine("};");
      hWriter.WriteLine();
      hWriter.Flush();
    }

    public HashSet<string> getUsedTypes()
    {
      var typesUsed = new HashSet<string>();
      foreach (var x in types)
      {
        typesUsed.Add(x.typeInfo.TypeName);
      }
      return typesUsed;
    }

    public bool IsTypedef()
    {
      return false;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      //Serializer & operator<< (Serializer & s, const ATCMessageHeader&m) {
      //  // Serialize Sequence header : no_marker, optionals[messageRefNumber], defaults[logicalAck=notRequired]
      //  s.stream.push_back(m.messageRefNumber.isInitialized() ? 1 : 0, 1);
      //  s.stream.push_back(m.logicalAck.isInitialized() ? 1 : 0, 1);   // Write a 1 if not using default.
      //                                                                 // Serialize Element
      //  s << m.messageIdNumber();
      //  if (m.messageRefNumber.isInitialized()) s << m.messageRefNumber();
      //  s << m.dateTime();
      //  if (m.logicalAck.isInitialized()) s << m.logicalAck();
      //  return s;
      //}
      hWriter.WriteLine("Serializer & operator<< (Serializer & s, const {0} & m) {{", typeName_);
      if (isExtensible())
      {
        bool extended = false; // TODO: set to true if extended
        hWriter.WriteLine("  s.stream << {0};", extended ? "true" : "false");
        hWriter.WriteLine("  s.stream.push_back(m.{0}.isInitialized() ? 1 : 0, 1);");
      }
      foreach (var item in types)
      {
        hWriter.WriteLine("  s.stream.push_back(m.{0}.isInitialized() ? 1 : 0, 1);");

        //Serializer & operator >> (Serializer & s, ATCMessageHeader & m) {
        //  // no extension marker
        //  // optional/default
        //  // serializeSequence(s, no_marker, optionals[messageRefNumber], defaults[logicalAck=notRequired]);
        //  bool has_messageRefNumber = (s.stream.pop_front(1) == 1);  // OPTIONAL; 0 = not used, 1 = present
        //  bool has_logicalAck = (s.stream.pop_front(1) == 1);  // DEFAULT; 0 = default, 1 = present
        //  s >> m.messageIdNumber();
        //  if (has_messageRefNumber) s >> m.messageRefNumber();
        //  s >> m.dateTime();
        //  if (has_logicalAck) s >> m.logicalAck();
        //  return s;
        //}
        throw new NotImplementedException();
      }
    }
  }

  public class Choice : ExtensibleType
  {
    public string ModuleName { get; private set; }
    public class Element
    {
      public Element(string moduleName) { typeInfo = new TypeInfo(moduleName); }
      public TypeInfo typeInfo;
      public bool optional; // unused
      public string identifier;
    }

    string Asn1Type.Name { get { return typeName_; } }
    private bool extensible_;
    private string typeName_;
    // private List<Tuple<string, TypeInfo>> choiceList_;
    private List<Element> choiceList_;

    public void setExtensible() { extensible_ = true; }

    public Choice(string typeName, string moduleName)
    {
      typeName_ = IdHelper.Id(typeName);
      ModuleName = moduleName;
    }

    // ChoiceType ::= CHOICE "{" AlternativeTypeLists "}"
    // AlternativeTypeLists::= RootAlternativeTypeList | RootAlternativeTypeList  ","    ExtensionAndException ExtensionAdditionAlternatives    OptionalExtensionMarke
    // RootAlternativeTypeList ::= AlternativeTypeList
    // AlternativeTypeList ::= NamedType  | AlternativeTypeList "," NamedType 
    public void Read(Parser parser, Context context)
    {
      List<Asn1Type> typesDefined_ = context.TypesByModule[ModuleName];
      parser.Expect("{");
      choiceList_ = new List<Element>();
      TypeInfo type;
      string identifier;
      do
      {
        identifier = parser.NextToken();
        type = new TypeInfo(ModuleName);
        if (identifier == "...")
        {
          setExtensible();
        }
        else
        {
          type.Read(parser, context);
          var info = new Element(ModuleName);
          info.identifier = identifier;
          info.typeInfo = type;
          choiceList_.Add(info);
        }
        identifier = parser.NextToken();
      } while (identifier == ",");
      if (identifier != "}") throw new Exception(string.Format("Invalid definition format. Expected } keyword, found {0}", identifier));
    }

    public bool isExtensible()
    {
      return extensible_;
    }

    public void Write(StreamWriter cxxWriter, StreamWriter hWriter)
    {
      hWriter.WriteLine("class {0} : public Asn1::Choice {{", typeName_);
      hWriter.WriteLine("public:");
      hWriter.WriteLine("  typedef {0} PrimitiveType;", typeName_);
      hWriter.WriteLine("  enum {");
      foreach (var choice in choiceList_)
      {
        hWriter.WriteLine("    {0}_idx = {1}{2}", choice.identifier, choice.typeInfo.Tag, choice == choiceList_.Last() ? "" : ",");
      }
      hWriter.WriteLine("  };");
      hWriter.WriteLine("  virtual bool extensible() const {{ return {0}; }}", isExtensible() ? "true" : "false");
      foreach (var choice in choiceList_)
      {
        string type;
        if ("SEQUENCE OF" == choice.typeInfo.TypeName)
        {
          // hWriter.WriteLine("  typedef {1} SequenceOf_{2};", item.typeInfo.InnerType.Name, (item.typeInfo.InnerType as SequenceOf).InnerTypeName);
          type = choice.typeInfo.InnerType.Name + " ";
        }
        else
        {
          if (choice.typeInfo.ModuleName != ModuleName)
          {
            type = choice.typeInfo.ModuleName + "::" + choice.typeInfo.TypeName;
          }
          else {
            type = choice.typeInfo.TypeName;
          }
        }
        hWriter.WriteLine("  Option<{0}_idx, {1}>::type {0}();", choice.identifier, type);
        //ProtectedGroundPDUs::Option<ProtectedGroundPDUs::abortUser_idx, PMCPDLCUserAbortReason>::type ProtectedGroundPDUs::abortUser()
        //{
        //  return Option<abortUser_idx, PMCPDLCUserAbortReason>::getElement(*this);
        //}
        cxxWriter.WriteLine("{0}::Option<{0}::{1}_idx, {2}>::type {0}::{1}()", typeName_, choice.identifier, type);
        cxxWriter.WriteLine("{");
        cxxWriter.WriteLine("  return Option<{0}_idx, {1}>::getElement(*this);", choice.identifier, type);
        cxxWriter.WriteLine("}");
      }
      hWriter.WriteLine("  {0}() : Choice() {{ }}", typeName_);
      hWriter.WriteLine("};");
      hWriter.WriteLine();
      hWriter.Flush();
    }

    public HashSet<string> getUsedTypes()
    {
      var typesUsed = new HashSet<string>();
      foreach (var x in choiceList_)
      {
        typesUsed.Add(x.typeInfo.TypeName);
      }
      return typesUsed;
    }

    public bool IsTypedef()
    {
      return false;
    }

    public bool IsBasic()
    {
      return IsTypedef();
    }

    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      throw new NotImplementedException();
    }
  }

  public class Parser
  {
    private StreamReader reader;
    private Queue<string> tokens = new Queue<string>();
    private bool ReadSome()
    {
      while (tokens.Count() < 1)
      {
        string line = reader.ReadLine();
        if (line == null) return false;
        if (line.Contains("--")) line = line.Substring(0, line.IndexOf("--"));
        line = line.Trim();
        if (string.IsNullOrWhiteSpace(line)) continue;
        char quote = '\0';
        int first = 0;
        bool quoted = false;
        for (int i = 0; i < line.Length; ++i)
        {
          if (quoted && line[i] == quote) { tokens.Enqueue(line.Substring(first, i - first)); first = i + 1; quoted = false; continue; }
          if (quoted) continue;
          if (char.IsWhiteSpace(line[i]))
          {
            if (first == i) { ++first; continue; }
            tokens.Enqueue(line.Substring(first, i - first)); first = i + 1; continue;
          }
          if (line[i] == '\'' || line[i] == '\"')
          {
            if (first != i) { tokens.Enqueue(line.Substring(first, i - first)); }
            quoted = true; quote = line[i]; first = i + 1;
            continue;
          }
          if (",;[]{}<>():".Contains(line[i]))
          {
            if (first != i) tokens.Enqueue(line.Substring(first, i - first));
            if ((line + "XX").Substring(i, 3) == "::=")
            {
              tokens.Enqueue("::="); i += 2; first = i + 1; continue;
            }
            else
            {
              tokens.Enqueue("" + line[i]); first = i + 1; continue;
            }
          }
          if (line[i] == '.')
          {
            if (first != i) tokens.Enqueue(line.Substring(first, i - first));
            var lineXX = line.Substring(i) + "XX";
            if (lineXX.StartsWith("..."))
            {
              tokens.Enqueue("..."); i += 2;
            }
            else if (lineXX.StartsWith(".."))
            {
              tokens.Enqueue(".."); ++i;
            }
            else
            {
              tokens.Enqueue(".");
            }
            first = i + 1; continue;
          }
          if (i == line.Length - 1) { tokens.Enqueue(line.Substring(first)); }
        }
      }
      return true;
    }

    public Parser(StreamReader reader)
    {
      this.reader = reader;
    }

    public string NextToken()
    {
      if (ReadSome()) return tokens.Dequeue();
      return null;
    }

    public string PeekToken()
    {
      if (ReadSome()) return tokens.Peek();
      return null;
    }

    public bool PeekToken(string match)
    {
      return match == PeekToken();
    }

    public void Expect(string expected)
    {
      var token = NextToken();
      if (token != expected) throw new Exception(string.Format("Invalid definition format. Expected {0} keyword, found {1}", expected, token));
    }

    public bool HasToken(string match)
    {
      if (match == PeekToken())
      {
        NextToken();
        return true;
      }
      return false;
    }
  }

  class Program
  {
    static void Main(string[] args)
    {
      string fname = args[0];
      string destFolder = args[1];
      Console.WriteLine("Input file:" + fname);
      Console.WriteLine("Dest folder:" + destFolder);
      if (File.Exists(fname))
      {
        ConvertToCPlusPlus(fname);
      }
    }

    private static void ConvertToCPlusPlus(string fname)
    {
      List<Definitions> asnModule = new List<Definitions>();
      Context context = new Context();

      using (var reader = new StreamReader(fname))
      {
        Parser parser = new Parser(reader);
        var nextToken = parser.PeekToken();
        while (nextToken!=null) {
          Definitions definitions = new Definitions();
          definitions.Read(parser, context);
          asnModule.Add(definitions);
          nextToken = parser.PeekToken();
        }
      }
      string targetFilePath = Path.GetDirectoryName(fname);
      foreach (var definitions in asnModule)
      {
        var cxxFileName = targetFilePath + Path.DirectorySeparatorChar + definitions.ModuleName + ".cxx";
        var hFileName = targetFilePath + Path.DirectorySeparatorChar + definitions.ModuleName + ".h";
        using (var cxxwriter = File.CreateText(cxxFileName))
        using (var hwriter = File.CreateText(hFileName))
        {
          definitions.WriteDefinitions(cxxwriter, hwriter, context);
        }

        cxxFileName = targetFilePath + Path.DirectorySeparatorChar + definitions.ModuleName + "Serializer.cxx";
        hFileName = targetFilePath + Path.DirectorySeparatorChar + definitions.ModuleName + "Serializer.h";
        using (var cxxwriter = File.CreateText(cxxFileName))
        using (var hwriter = File.CreateText(hFileName))
        {
          definitions.WriteSerializers(cxxwriter, hwriter, context);
        }
      }
    }

  }
}
