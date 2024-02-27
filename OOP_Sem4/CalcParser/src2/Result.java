public class Result {
    public double res = 0;

    public char oper;
    public String rest;
    public Result right = null;
    public Result left = null;

    public Result(double a, String r, char sym, Result left, Result right) {
        this.res = a;
        this.rest = r;
        this.right = right;
        this.left = left;
        this.oper = sym;
    }
}
