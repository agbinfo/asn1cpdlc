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
      //hWriter.WriteLine("public:");
      //hWriter.WriteLine("  {0}() : Asn1IA5String() {{ }}", identifier_);
      //hWriter.WriteLine("  {0}(const {0}&src) : Asn1IA5String(src) {{ }}", identifier_);
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
      //hWriter.WriteLine("  {0}(const {0}&src) : BitString(src) {{ }}", typeName_);
      //hWriter.WriteLine("  const {0}& operator= (const {0}&src) {{", typeName_);
      //hWriter.WriteLine("    BitString::operator=(src);");
      //hWriter.WriteLine("    return *this;");
      //hWriter.WriteLine("  }");
      hWriter.WriteLine("  const {0}& operator= (const PrimitiveType v) {{", typeName_);
      hWriter.WriteLine("    BitString::operator=(v);");
      hWriter.WriteLine("    return *this;");
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
        hWriter.WriteLine("  s.stream << {0};", extended ? "true" : "false"); // extension bit
      }
      foreach (var item in types)
      {
        // if item is DEFAULT or OPTIONAL write true if using default or present; false otherwise
        if (item.optional)
        {
          hWriter.WriteLine("  s.stream << {0}.isInitialized();", item.typeInfo.TypeName);
        }
        // todo: add same for DEFAULT
      }

      foreach (var item in types)
      {
        hWriter.WriteLine("  s.stream.push_back(m.{0}.isInitialized() ? 1 : 0, 1);", item.typeInfo);

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
        // TODO: (2) throw new NotImplementedException();
      }
      if (isExtensible()) // && extended
      {
        // serialize extensible part
      }
      hWriter.WriteLine("};");
      hWriter.WriteLine();
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

      public bool extension { get; internal set; }
    }

    string Asn1Type.Name { get { return typeName_; } }
    private bool extensible_;
    private string typeName_;
    // private List<Tuple<string, TypeInfo>> choiceList_;
    private List<Element> choiceList_;

    public void setExtensible() { extensible_ = true; }
    public bool isExtensible() { return extensible_; }

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
      string identifier;
      do
      {
        identifier = parser.NextToken();
        if (identifier == "...")
        {
          setExtensible();
        }
        else
        {
          TypeInfo type;
          type = new TypeInfo(ModuleName);
          type.Read(parser, context);
          var info = new Element(ModuleName);
          info.identifier = identifier;
          info.typeInfo = type;
          info.extension = isExtensible();
          choiceList_.Add(info);
        }
        identifier = parser.NextToken();
      } while (identifier == ",");
      if (identifier != "}") throw new Exception(string.Format("Invalid definition format. Expected } keyword, found {0}", identifier));
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

    // 22. Encoding the choice type 
    // NOTE – (Tutorial) A choice type is encoded by encoding an index
    // specifying the chosen alternative. This is encoded
    // as for a constrained integer (unless the extension marker is present in
    // the choice type, in which case it is a normally small non-negative
    // whole number) and would therefore typically occupy a fixed length
    // bit-field of the minimum number of bits needed to encode the index.
    // (Although it could in principle be arbitrarily large.) This is followed
    // by the encoding of the chosen alternative, with alternatives that are
    // extension additions encoded as if they were the value of an open type
    // field. Where the choice has only one alternative, there is no encoding
    // for the index.
    public void WriteSerializers(StreamWriter cxxWriter, StreamWriter hWriter, Context context)
    {
      // TODO: (1) throw new NotImplementedException();
    }
  }

  class Program
  {
    static void Main(string[] args)
    {
      string fname = args[0];
      string destFolder = args.Count() > 1 ? args[1] : null;
      Console.WriteLine("Input file:" + fname);
      Console.WriteLine("Dest folder:" + destFolder);
      if (File.Exists(fname))
      {
        ConvertToCPlusPlus(fname, destFolder);
      }
    }

    private static void ConvertToCPlusPlus(string fname, string targetPath)
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
      string targetFilePath = string.IsNullOrEmpty(targetPath) ? Path.GetDirectoryName(fname) : targetPath;
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
