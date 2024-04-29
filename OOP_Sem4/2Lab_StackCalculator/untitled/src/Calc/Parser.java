package Calc;

import Except.CalcExceptions;
import operations.Oper;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

public abstract class Parser {
    public enum CommandsEn {
        PLUS, MINUS, MULT, DIV, SQRT,
        POP, PUSH, PRINT, DEFINE
    }
    ExecutionContext ec;
    public Parser() {
        Stack<Double> stack = new Stack<>();
        Map<String, Double> map = new HashMap<>();
        ec = new ExecutionContext(stack, map);
    }

    public void parsing(List<String> commands) throws CalcExceptions {
        for(String cmd: commands) {
            String[] args = cmd.split("\\s+");
            if (args.length == 0) {
                continue;
            }
            CommandValidator.validateCommand(args[0].toUpperCase(), args.length);
            // свитч для длинны аргументов комманды?
            switch (args[0].toUpperCase()) {
                case "DEFINE":
                    System.out.println("define\n");
                    break;
                case "PUSH":
                    System.out.println("push\n");
                    break;
                case "POP":
                    System.out.println("pop\n");
                    break;
                case "PRINT":
                    System.out.println("print\n");
                    break;
                case "SQRT":
                    System.out.println("sqrt\n");
                    break;
                case "+":
                    System.out.println("+\n");
                    break;
                case "-":
                    System.out.println("-\n");
                    break;
                case "*":
                    System.out.println("*\n");
                    break;
                case "/":
                    System.out.println("/\n");
                    break;
                default:
                    break;

            }
        }
    }

    protected abstract Oper calculating(CommandsEn type, Object[] args);

//    private void checkComand(String cmd, int len) throws CalcExceptions {
//        switch (cmd) {
//            case "DEFINE":
//                if (len != 3) {
//                    throw new CalcExceptions("Error: Invalid number of command arguments: " + cmd);
//                }
//                break;
//            case "PUSH":
//                if (len != 2) {
//                    throw new CalcExceptions("Error: Invalid number of command arguments: " + cmd);
//                }
//                break;
//            case "POP":
//            case "PRINT":
//            case "SQRT":
//            case "+":
//            case "-":
//            case "*":
//            case "/":
//                if (len != 1) {
//                    throw new CalcExceptions("Error: Invalid number of command arguments: " + cmd);
//                }
//                break;
//            default:
//                if (cmd.charAt(0) != '#') {
//                    throw new CalcExceptions("Error: Unknown command: " + cmd);
//                }
//                    break;
//
//        }
//    }
}
