package operations;

import Except.CalcExceptions;

public class Mult extends Oper {
    public double Calculate(double[] argv) throws CalcExceptions {
        if (argv.length != 2) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        return argv[0] * argv[1];
    }
}
