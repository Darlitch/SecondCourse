package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Stack;

import static java.lang.Math.abs;

public class Div extends Oper {
//    private Div(){
//        super();
//    }
//    public static Div newInstance() {
//        return new Div();
//    }
    public void doOper(Object[] args) throws CalcExceptions {
        if (args.length != 1) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        ExecutionContext ec = (ExecutionContext)args[0];
        Stack<Double> stack = ec.getStack();
        if (stack.size() < 2) {
            throw new CalcExceptions("Error: There are not enough elements in the stack to perform the operation");
        }
        double arg1 = stack.pop();
        double arg2 = stack.pop();
        if (abs(arg1 - 0) < 0.00001) {
            throw new CalcExceptions("Error: Division by zero");
        }
        stack.push(arg2/arg1);
    }
}
