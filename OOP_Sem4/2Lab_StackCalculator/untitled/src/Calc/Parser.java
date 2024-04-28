package Calc;

import Except.CalcExceptions;

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
            switch (args[0].toUpperCase()) {
                case "DEFINE":
                    break;
                case "PUSH":
                    break;
                case "POP":
                    break;
                case "PRINT":
                    break;
                case "SQRT":
                    break;
                case "+":
                    break;
                case "-":
                    break;
                case "*":
                    break;
                case "/":
                    break;
                default:
                    if (args[0].charAt(0) == '#') {
                        break;
                    } else {
                        throw new CalcExceptions("Error: Unknown command: " + args[0]);
                    }

            }
        }
    }

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
