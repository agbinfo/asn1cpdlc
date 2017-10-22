using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Asn1ToCPlusPlusConsole
{
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
        else
        {
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
          else if (nextToken.Equals("ENUMERATED"))
          {
            Enumerated enumerated = new Enumerated(keyword, ModuleName);
            enumerated.Read(parser);
            context.Types.Add(enumerated);
          }
          else if (nextToken.Equals("RELATIVE-OID"))
          {
            RelativeOid oid = new RelativeOid(keyword, ModuleName);
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
      hWriter.WriteLine("// {0}Serializer.h", ModuleName);
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
      hWriter.WriteLine("namespace Asn1 {");
      hWriter.WriteLine("namespace Serializer {");
      hWriter.WriteLine();

      List<Asn1Type> typesDefined = context.TypesByModule[ModuleName];
      foreach (var type in typesDefined)
      {
        if (!type.IsTypedef())
        {
          type.WriteSerializers(cxxWriter, hWriter, context);
        }
      }

      hWriter.WriteLine("} } // end namespace Asn1::Serializer");
    }
  };
}
