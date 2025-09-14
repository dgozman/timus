import java.io.*;
import java.math.*;
import java.util.*;

public class timus1967 {
    static BufferedReader in;
    static PrintWriter out;

    public static void main(String[] args) throws Exception {
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();
    }

    public static void solve() throws Exception {
      int n = Integer.parseInt(in.readLine().trim());
      StringBuilder builder = new StringBuilder();
      for (int i = 1; i <= n; i++)
        builder.append(BigInteger.valueOf(i).toString(2));
      String bits = builder.toString();
      int[][] result = build(bits);
      int[] from = result[0];
      int[] to = result[1];
      int count = 0;
      for (int i = 0; i < from.length; i++) {
        if (from[i] != to[i])
          count++;
      }
      out.println(count);
      for (int i = 0; i < from.length; i++) {
        if (from[i] != to[i])
          out.println((to[i] - from[i]) + " ");
      }
      out.println();


      // for (int n = 5; n <= 5; n++) {
      //   StringBuilder builder = new StringBuilder();
      //   for (int i = 1; i <= n; i++)
      //     builder.append(BigInteger.valueOf(i).toString(2));
      //   String bits = builder.toString();
      //   // String[] res = rec(bits, new String[] { "", "", "" }, 0, 0);
      //   String solved = find(bits);
      //   out.println(n + " => " + solved);
      //   // out.println(n + " => " + res[2] + " or " + res[1] + " solved=" + solved);
      //   out.flush();
      // }
    }

    static class Num {
      int from, to;
      String dec;

      public Num(String bits, int from, int to) {
        this.from = from;
        this.to = to;
        if (from == to)
          this.dec = "";
        else
          this.dec = new BigInteger(bits.substring(from, to), 2).toString(10);
      }
    }

    public static int[][] build(String bits) {
      int count = 0;
      for (int i = 0; i < bits.length(); i++) {
        if (bits.charAt(i) == '1')
          count++;
      }

      int[] from = new int[count];
      int[] to = new int[count];
      int curFrom = 0;
      count = 0;
      for (int i = 0; i < bits.length(); i++) {
        if (bits.charAt(i) == '1') {
          if (curFrom < i) {
            from[count] = curFrom;
            to[count] = i;
            count++;
          }
          curFrom = i;
        }
      }
      if (curFrom < bits.length()) {
        from[count] = curFrom;
        to[count] = bits.length();
        count++;
      }

      String cur = toDec(bits, from[count - 1], to[count - 1]);
      for (int i = count - 1; i > 0; i--) {
        String prev = toDec(bits, from[i - 1], to[i - 1]);
        String joined = toDec(bits, from[i - 1], to[i]);
        int len = cur.length() + prev.length();
        if (joined.length() < len || (joined.length() == len && joined.compareTo(prev + cur) < 0)) {
          from[i] = to[i];
          to[i - 1] = to[i];
          cur = joined;
        } else {
          cur = prev;
        }
      }
      return new int[][] { from, to };
    }

    public static String sub(String bits, int from, int to) {
      if (from == to)
        return "";
      return bits.substring(from, to);
    }

    public static String dec(String bin) {
      if (bin.isEmpty())
        return "";
      return new BigInteger(bin, 2).toString(10);
    }

    public static String toDec(String bits, int from, int to) {
      if (from == to)
        return "";
      return new BigInteger(bits.substring(from, to), 2).toString(10);
    }

    public static boolean better(String a, String b) {
      if (a.length() != b.length())
        return a.length() < b.length();
      return a.compareTo(b) < 0;
    }

    public static String[] best(String[] ap, String[] bp) {
      return better(ap[0], bp[0]) ? ap : bp;
    }

    public static String[] join(String[] a, String b, String c) {
      if (b.isEmpty())
        return a;
      if (a[0].isEmpty())
        return new String[] { b, b, c };
      return new String[] { a[0] + b, a[1] + "|" + b, a[2] + "|" + c };
    }

    public static String[] rec(String bits, String[] s, int from, int index) {
      if (index == bits.length()) {
        String bin = sub(bits, from, index);
        return join(s, dec(bin), bin);
      }
      if (bits.charAt(index) == '0')
        return rec(bits, s, from, index + 1);
      String bin = sub(bits, from, index);
      return best(rec(bits, s, from, index + 1), rec(bits, join(s, dec(bin), bin), index, index + 1));
    }
}
