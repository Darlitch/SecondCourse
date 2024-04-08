//  из исо во внутренню, из внутренней в исо, и прибавить знаковые секунды
public class TimePoint {
    private static long dateInSec;
    private static final int[] dayInM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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

    private int FindMounth(int days, int year) {
        int i = 0;
        while (days > dayInM[i]) {
            if(i == 1 && year % 4 == 0) {
                days -= 1;
            }
            days -= dayInM[i];
            i++;
        }
        return i;
    }

    private int FindDay(int days, int year) {
        int i = 0;
        while (days > dayInM[i]) {
            if(i == 1 && year % 4 == 0) {
                days -= 1;
            }
            days -= dayInM[i];
            i++;
        }
        return days;
    }

    public String to_str() {
        int year, month, day, h, min, sec;
        sec = (int)(dateInSec % 60);
        min = (int)(dateInSec / 60 % 60);
        h = (int)(dateInSec / (60*60) % 24);
        year = (int)(dateInSec / (365.25 * 24 * 60 * 60));
        int days = (int)(dateInSec / (24 * 60 * 60) % 365.25);
        month = FindMounth(days, year);
        day = FindDay(days, year);
//        day = (int)(dateInSec / (24 * 60 * 60 * 365.25 * 12) % (dayInM[month]));
        String a = String.format("%04d-%02d-%02dT%02d:%02d:%02d", year, month, day, h, min, sec);
        return a;
    }
    private static void DateToSecond(int year, int month, int day, int h, int min, int sec) {
        dateInSec = (long)(year * 365.25);
//        dateInSec += (year-1) / 4;
        for (int i = 0; i < month; ++i) {
//            if (i == 4 || i == 6 || i == 9 || i == 11) {
//                dateInSec += 30;
//            } else if (i == 2 && (year % 4 == 0)) {
//                dateInSec += 29;
//            } else if (i == 2)  {
//                dateInSec += 28;
//            } else {
//                dateInSec += 31;
//            }
            dateInSec += dayInM[i];
            if (year % 4 == 0 && i == 1) {
                dateInSec += 1;
            }
        }
        dateInSec += day;
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
//        if ((month == 1 || month == 3 || month == 5 || month == 7
//            || month == 8 || month == 10 || month == 12) && day > 31) {
//            throw new DateExceptions("Incorrect day: Max in this month: 31");
//        } else if (month == 2 && (year % 4 == 0) && day > 29) {
//            throw new DateExceptions("Incorrect day: Max in this month in this year: 29");
//        } else if (month == 2 && day > 28) {
//            throw new DateExceptions("Incorrect day: Max in this month in this year: 28");
//        } else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
//            throw new DateExceptions("Incorrect day: Max in this month: 30");
//        }
        if (month == 2 && day > 29) {
            throw new DateExceptions("Incorrect day");
        } else if (day > dayInM[month-1]) {
            throw new DateExceptions("Incorrect day");
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
