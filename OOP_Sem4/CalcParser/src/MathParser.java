public class MathParser {
    public MathParser() {}
    public double Parse(String s) throws Exception {
        Result result = PlusMinus(s);
        if (!result.rest.isEmpty()) {
            System.err.println("Error: The parsing is not finished");
            System.err.println("rest: " + result.rest);
        }
        return result.res;
    }

    public Result PlusMinus(String s) throws Exception {
        Result curr = MultDiv(s);
        Result curr2 = curr;
        double res = curr.res;
        while (!curr2.rest.isEmpty()) {
            if (curr2.rest.charAt(0) != '+' && curr2.rest.charAt(0) != '-') {
                break;
            }
            char temp = curr2.rest.charAt(0);
            curr2 = MultDiv(curr2.rest.substring(1));
            if (temp == '+') {
                res += curr2.res;
            } else {
                res -= curr2.res;
            }
        }
        return new Result(res, curr2.rest, curr2, curr);
    }

    public Result MultDiv(String s) throws Exception {
        Result curr = Brackets(s);
        Result curr2 = curr;
        double res = curr.res;
        while (!curr2.rest.isEmpty()) {
            if (curr2.rest.charAt(0) != '*' && curr2.rest.charAt(0) != '/') {
                break;
            }
            char temp = curr2.rest.charAt(0);
            curr2 = Brackets(curr2.rest.substring(1));
            if (temp == '*') {
                res *= curr2.res;
            } else {
                res /= curr2.res;
            }
        }
        return new Result(res, curr2.rest, curr, curr2);
    }

    public Result Brackets(String s) throws Exception {
        if (s.charAt(0) == '(') {
            Result result = PlusMinus(s.substring(1));
            if (!result.rest.isEmpty() && result.rest.charAt(0) == ')') {
                result.rest = result.rest.substring(1);
            } else {
                System.err.println("Error: not close bracket");
            }
            return result;
        }
        Result result2 = Num(s);
        return new Result(result2.res, result2.rest, null, null);
    }

    public Result Num(String s) throws Exception {
        int i = 0;
        int dotCount = 0;
        boolean negative = false;
        if (s.charAt(0) == '-') {
            negative = true;
            s = s.substring(1);
        }
        while (i < s.length() && (Character.isDigit(s.charAt(i)) || s.charAt(i) == '.')) {
            if (s.charAt(i) == '.' && ++dotCount > 1) {
                throw new Exception("not valid num: " + s.substring(0, i + 1));
            }
            i++;
        }
        if (i == 0) {
            throw new Exception("can`t get valid num");
        }
        double dNum = Double.parseDouble(s.substring(0, i));
        if (negative) {
            dNum = -dNum;
        }
        return new Result (dNum, s.substring(i), null, null);
    } 
}
