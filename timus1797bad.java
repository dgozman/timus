import java.math.*;
import java.util.*;
import java.io.*;

public class timus1797 {

	protected void run() throws IOException {
		Locale.setDefault(Locale.US);
		//in = new BufferedReader(new FileReader(""));
		in = new BufferedReader(new InputStreamReader(System.in));
		out = new PrintWriter(System.out);

    int tests = nextInt();
    for (int test = 0; test < tests; test++) {
      BigInteger x = nextBigInteger();
      BigInteger y = nextBigInteger();
      if (x.compareTo(y) > 0) {
        BigInteger t = x;
        x = y;
        y = t;
      }
      BigInteger l = nextBigInteger();
      BigInteger r = nextBigInteger();

      BigInteger a = solve(x, y, r);
      BigInteger b = solve(x, y, l.subtract(BigInteger.ONE));
      out.println(a.subtract(b).toString());
    }

		in.close();
		out.close();
	}

	private BigInteger solve(BigInteger x, BigInteger y, BigInteger n) {
		if (y.equals(x)) {
			return n.divide(x);
		}

    // long k = (x + (y - x) - 1) / (y - x)
		BigInteger k = y.subtract(BigInteger.ONE).divide(y.subtract(x));
		BigInteger t = n.divide(x);

		if (t.compareTo(k) < 0) {
			if (t.equals(BigInteger.ZERO)) {
				return BigInteger.ZERO;
			}
			// return (y - x) * sum(t - 1) + (t - 1) + Math.min(n, t * y) - t * x + 1;
			return y.subtract(x).multiply(sum(t.subtract(BigInteger.ONE))).add(t).add(n.min(t.multiply(y))).subtract(t.multiply(x));
		} else {
			// return (y - x) * sum(k - 1) + (k - 1) + (n - k * x + 1);
			return y.subtract(x).multiply(sum(k.subtract(BigInteger.ONE))).add(k).add(n).subtract(k.multiply(x));
		}
	}

	private BigInteger sum(BigInteger x) {
		if (!x.testBit(0)) {
			BigInteger t = x.divide(BigInteger.valueOf(2));
			return t.multiply(x.add(BigInteger.ONE));
		} else {
			BigInteger t = x.add(BigInteger.ONE).divide(BigInteger.valueOf(2));
			return t.multiply(x);
		}
	}

	public static void main(String[] args) {
		new Thread(new Runnable(){
			public void run() {
				try {
					new timus1797().run();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}}).start();
	}

	BufferedReader in;
	StringTokenizer tokenizer;
	PrintWriter out;

	private int nextInt() throws IOException {
		return Integer.parseInt(nextString());
	}

	private long nextLong() throws IOException {
		return Long.parseLong(nextString());
	}

	private BigInteger nextBigInteger() throws IOException {
		return new BigInteger(nextString());
	}

	private double nextDouble() throws IOException {
		return Double.parseDouble(nextString());
	}

	private String nextString() throws IOException {
		while (tokenizer == null || !tokenizer.hasMoreTokens()) {
			tokenizer = new StringTokenizer(in.readLine());
		}
		return tokenizer.nextToken();
	}

	private String nextLine() throws IOException {
		if (tokenizer != null && tokenizer.hasMoreTokens()) {
			System.exit(1);
		}
		tokenizer = null;
		return in.readLine();
	}
}


/*

for i=0..(x/gcd(x,y))-1
(n-iy)//x = (n+kxi-iy)//x - ki = (n+i(kx-y))//x - ki

(p+iq)//x

tq>=x, t>=2
p//x, (p+q)//x, (p+2q)//x, ... (p+(t-1)q)//x


*/