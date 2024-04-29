package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Stack;

public class Sqrt extends Oper {
    public void doOper(Object[] args) throws CalcExceptions {
//        if (args.length != 1) {
//            throw new CalcExceptions("Error: invalid count of args");
//        }
//        double arg1 = (double)args[0];
//        return Math.sqrt(arg1);
        if (args.length != 1) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        ExecutionContext ec = (ExecutionContext)args[0];
        Stack<Double> stack = ec.getStack();
        if (stack.isEmpty()) {
            throw new CalcExceptions("Error: There are not enough elements in the stack to perform the operation");
        }
        stack.push(Math.sqrt(stack.pop()));
    }
}
