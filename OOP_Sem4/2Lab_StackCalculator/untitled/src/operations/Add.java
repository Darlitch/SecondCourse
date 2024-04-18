package operations;

import Except.CalcExceptions;

public class Add extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 2) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        double arg1 = (double)args[0];
        double arg2 = (double)args[1];
        return arg1 + arg2;
    }
}
