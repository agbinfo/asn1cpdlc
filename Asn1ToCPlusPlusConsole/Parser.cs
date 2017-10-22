using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Asn1ToCPlusPlusConsole
{
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
}
