package operations;

import Except.CalcExceptions;

public class Div extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 2) {
            throw new CalcExceptions("Error: invalid count of args");
        }
        double arg1 = (double)args[0];
        double arg2 = (double)args[1];
        if (arg1 == 0) {
            throw new CalcExceptions("Error: division by zero");
        }
        return arg1 / arg2;
    }
}
