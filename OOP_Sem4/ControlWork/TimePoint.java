//  из исо во внутренню, из внутренней в исо, и прибавить знаковые секунды
public class TimePoint {
    private static long dateInSec;
//    private static int year;
//    private static int month;
//    private static int day;
//    private static int h;
//    private static int min;
//    private static int sec;

    public TimePoint(long sumSec) {
        dateInSec = sumSec;
    }

    public static TimePoint from_str(String date) throws DateExceptions {
        int h = 0;
        int min = 0;
        int sec = 0;
        if (date.length() != 10 && date.length() != 19) {
            throw new DateExceptions("Incorrect DateFormat");
        }
        int year = Integer.parseInt(date.substring(0,4));
        int month = Integer.parseInt(date.substring(5,7));
        int day = Integer.parseInt(date.substring(8,10));
        if (date.length() == 19) {
            h = Integer.parseInt(date.substring(11,13));
            min = Integer.parseInt(date.substring(14,16));
            sec = Integer.parseInt(date.substring(17,19));
        }
        CheckDate(year, month, day, h, min, sec);
        DateToSecond(year, month, day, h, min, sec);
        return new TimePoint(dateInSec);
    }

    void add (int seconds) {
        dateInSec += seconds;
    }

//    public String to_str() {
//        String a = String.format("%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, h, min, sec);
//        return a;
//    }
    private static void DateToSecond(int year, int month, int day, int h, int min, int sec) {
        dateInSec = year * 365;
        dateInSec += (year-1) / 4;
        for (int i = 1; i < month; ++i) {
            if (i == 4 || i == 6 || i == 9 || i == 11) {
                dateInSec += 30;
            } else if (i == 2 && (year % 4 == 0)) {
                dateInSec += 29;
            } else if (i == 2)  {
                dateInSec += 28;
            } else {
                dateInSec += 31;
            }
        }
        dateInSec += day - 1;
        dateInSec = ((dateInSec * 24 + h) * 60 + min) * 60 + sec;
        System.out.println(dateInSec);
    }
    private static void CheckDate(int year, int month, int day, int h, int min, int sec) throws DateExceptions {
        if (month < 1 || month > 12) {
            throw new DateExceptions("Incorrect month");
        }
        if (day < 1) {
            throw new DateExceptions("Incorrect day");
        }
        if ((month == 1 || month == 3 || month == 5 || month == 7
            || month == 8 || month == 10 || month == 12) && day > 31) {
            throw new DateExceptions("Incorrect day: Max in this month: 31");
        } else if (month == 2 && (year % 4 == 0) && day > 29) {
            throw new DateExceptions("Incorrect day: Max in this month in this year: 29");
        } else if (month == 2 && day > 28) {
            throw new DateExceptions("Incorrect day: Max in this month in this year: 28");
        } else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            throw new DateExceptions("Incorrect day: Max in this month: 30");
        }
        if (h < 0 || h > 23) {
            throw new DateExceptions("Incorrect hour");
        }
        if (min < 0 || min > 59) {
            throw new DateExceptions("Incorrect minutes");
        }
        if (sec < 0 || sec > 59) {
            throw new DateExceptions("Incorrect second");
        }
    }


}
