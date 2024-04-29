package Calc;

import Except.CalcExceptions;

import java.io.IOException;
import java.util.InputMismatchException;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> commands;
        try {
            if (args.length == 1) {
                commands = InputProcessing.Processing(args[0]);
            } else if (args.length == 0) {
                commands = InputProcessing.Processing();
            } else {
                throw new CalcExceptions("Error: Invalid number of arguments");
            }
//            Parser parser = new Parser();
//            parser.parsing(commands);
//            for (String command: commands) {
//                System.out.println(command);
//            }
        } catch (CalcExceptions e) {
            System.out.println(e.getMessage());
        } catch (IOException | InputMismatchException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}