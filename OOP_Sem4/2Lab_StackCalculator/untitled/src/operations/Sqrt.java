package operations;

import Except.CalcExceptions;

public class Sqrt extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 1) {
            throw new CalcExceptions("Error: invalid count of args");
        }
        double arg1 = (double)args[0];
        return Math.sqrt(arg1);
    }
}
