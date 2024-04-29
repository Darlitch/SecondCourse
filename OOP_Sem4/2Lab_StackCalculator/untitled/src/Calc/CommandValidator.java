package Calc;

import Except.CalcExceptions;

import java.util.HashMap;
import java.util.Map;

public class CommandValidator {
    private static final Map<String, Integer> commandLengths = new HashMap<>();

    static {
        commandLengths.put("DEFINE", 3);
        commandLengths.put("PUSH", 2);
        commandLengths.put("POP", 1);
        commandLengths.put("PRINT", 1);
        commandLengths.put("SQRT", 1);
        commandLengths.put("+", 1);
        commandLengths.put("-", 1);
        commandLengths.put("*", 1);
        commandLengths.put("/", 1);
    }

    public static void validateCommand(String cmd, int len) throws CalcExceptions {
        if (commandLengths.containsKey(cmd)) {
            int expectedLength = commandLengths.get(cmd);
            if (len != expectedLength) {
                throw new CalcExceptions("Error: Invalid number of command arguments: " + cmd);
            }
        } else if (cmd.charAt(0) != '#') {
            throw new CalcExceptions("Error: Unknown command: " + cmd);
        }
    }

    public static int CmdLength(String cmd) {
        return commandLengths.get(cmd);
    }
}