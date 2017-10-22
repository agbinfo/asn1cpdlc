using System.Collections.Generic;
using System.Linq;

namespace Asn1ToCPlusPlusConsole
{
  public class Context
  {
    public Context()
    {
      // Identifiers = new List<Asn1TypedIdentifier>();
      TypesByModule = new Dictionary<string, List<Asn1Type>>();
      ImportsByModule = new Dictionary<string, List<Imports>>();
    }

    // public List<Asn1TypedIdentifier> Identifiers { get; }
    public List<Asn1Type> Types { get { return TypesByModule[moduleName_]; } }
    public Dictionary<string, List<Asn1Type>> TypesByModule;
    private string moduleName_;
    public List<Imports> Imports { get { return ImportsByModule[moduleName_]; } }
    public Dictionary<string, List<Imports>> ImportsByModule;

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
      if (TypesByModule[moduleName_].Any(type => type.Name == typeName))
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
}
