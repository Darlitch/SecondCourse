import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws DateExceptions {
        Scanner scan = new Scanner(System.in);
        System.out.println("Введите дату в формате iso 8601:");
        String date = scan.nextLine();
        try {
            TimePoint tp = TimePoint.from_str(date);
//            String a = tp.to_str();
//            System.out.println(a);
        } catch (DateExceptions message) {
            message.printStackTrace();
        }
    }

}
