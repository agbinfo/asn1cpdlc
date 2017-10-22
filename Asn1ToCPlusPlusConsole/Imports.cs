using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Asn1ToCPlusPlusConsole
{
  public class Imports
  {
    public List<Tuple<string, List<string>>> symbolsFromModules_ = new List<Tuple<string, List<string>>>();

    public Imports()
    {
    }

    public void Read(Parser parser)
    {
      while (!parser.PeekToken(";"))
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
}
