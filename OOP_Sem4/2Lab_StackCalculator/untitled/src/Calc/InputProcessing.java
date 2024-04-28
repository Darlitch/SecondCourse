package Calc;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class InputProcessing {
//    List<String> commands;
////    String filename;
//    ExecutionContext ec;
//
//    public Parser(String filename) {
//        this.filename = filename;
//        Stack<Double> stack = new Stack<>();
//        Map<String, Double> map = new HashMap<>();
//        ec = new ExecutionContext(stack, map);
//    }

    public static List<String> Processing(String filename) throws IOException {
        List<String> commands = new ArrayList<>();
        try( BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;
            while (((line = reader.readLine()) != null)) {
                commands.add(line);
            }
        }
        return commands;
    }

    public static List<String> Processing() throws InputMismatchException {
        List<String> commands = new ArrayList<>();
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.println("Введите команды построчно:");
            System.out.println("Для завершения введите end.");
            String line = scanner.nextLine();
            while (!line.equals("end")) {
                commands.add(line);
                line = scanner.nextLine();

            }
        }
        return commands;
    }
}
