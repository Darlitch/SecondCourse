package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Stack;

public class Pop extends Oper {
    public void doOper(Object[] args) throws CalcExceptions {
        if (args.length != 1) {
            throw new CalcExceptions("Error: invalid count of args");
        }
//        Stack<Double> stack = (Stack<Double>)args[0];
        ExecutionContext ec = (ExecutionContext)args[0];
        Stack<Double> stack = ec.getStack();
        stack.pop();
    }
}
