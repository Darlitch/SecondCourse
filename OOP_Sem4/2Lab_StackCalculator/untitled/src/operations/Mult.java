package operations;

import Except.CalcExceptions;

public class Mult extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 2) {
            throw new CalcExceptions("Error: invalid count of args");
        }
        double arg1 = (double)args[0];
        double arg2 = (double)args[1];
        return arg1 * arg2;
    }
}
