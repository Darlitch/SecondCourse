package Calc;

import Except.CalcExceptions;
import operations.*;

import java.util.Stack;

public class Calculator extends Parser {
    protected Oper calculating(String cmd) throws CalcExceptions {
        Oper oper = null;
        oper = switch (cmd) {
            case "DEFINE" -> new Define();
            case "PUSH" -> new Push();
            case "POP" -> new Pop();
            case "PRINT" -> new Print();
            case "SQRT" -> new Sqrt();
            case "+" -> new Add();
            case "-" -> new Sub();
            case "*" -> new Mult();
            case "/" -> new Div();
            default -> throw new CalcExceptions("Error CalcOper: Unknown command: " + oper);
        };
        return oper;
    }

}

