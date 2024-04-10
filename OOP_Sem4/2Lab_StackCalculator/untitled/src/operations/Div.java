package operations;

import Except.CalcExceptions;

public class Div extends Oper {
    public double Calculate(double[] argv) throws CalcExceptions {
        if (argv.length != 2) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        if (argv[1] == 0) {
            throw new CalcExceptions("Error: division by zero");
        }
        return argv[0] / argv[1];
    }
}
