public class MathParser {
    public MathParser() {}
    public double Parse(String s) throws Exception {
        Result result = PlusMinus(s);
        if (!result.rest.isEmpty()) {
            System.err.println("Error: The parsing is not finished");
            System.err.println("rest: " + result.rest);
        }
        return Tree(result);
    }

    public double Tree(Result result) {
        double res = 0;
        if (result.oper == '+') {
            res = Tree(result.right) + Tree(result.left);
        } else if (result.oper == '-') {
            res = Tree(result.right) - Tree(result.left);
        } else if (result.oper == '*') {
            res = Tree(result.right) * Tree(result.left);
        } else if (result.oper == '/') {
            res = Tree(result.right) / Tree(result.left);
        } else if (result.oper == 'n') {
            res = result.res;
        }
        return res;
}
    public Result PlusMinus(String s) throws Exception {
        Result curr = MultDiv(s);
        Result curr2 = curr;
        char oper = 's';
//        double res = curr.res;
        while (!curr2.rest.isEmpty()) {
            if (curr2.rest.charAt(0) != '+' && curr2.rest.charAt(0) != '-') {
                return curr2;
            }
            curr2 = MultDiv(curr2.rest.substring(1));
            if (curr.rest.charAt(0) == '-') {
                oper = '-';
//                res += curr2.res;
            } else if (curr.rest.charAt(0) == '+'){
                oper = '+';
//                res -= curr2.res;
            } else {
                return curr2;
            }
        }
        return new Result(0, curr2.rest, oper, curr2, curr);
    }

    public Result MultDiv(String s) throws Exception {
        Result curr = Brackets(s);
        Result curr2 = curr;
        char oper = 's';
//        double res = curr.res;
        while (!curr2.rest.isEmpty()) {
            if (curr2.rest.charAt(0) != '*' && curr2.rest.charAt(0) != '/') {
                return curr2;
            }
            curr2 = Brackets(curr2.rest.substring(1));
            if (curr.rest.charAt(0) == '/') {
                oper = '/';
//                res *= curr2.res;
            } else if (curr.rest.charAt(0) == '*'){
                oper = '*';
//                res /= curr2.res;
            } else {
                return curr2;
            }
        }
        return new Result(0, curr2.rest, oper, curr, curr2);
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
        return new Result(result2.res, result2.rest, 'n', null, null);
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
        return new Result (dNum, s.substring(i), 'n', null, null);
    } 
}
