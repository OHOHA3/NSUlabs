package integration;

//f(x)=e^x * cos(x)  [5;7]
public class WorkWithFunction {
    private static int countOfSegments;
    private static double h;
    private static double delta;

    public static void setParameters(int numberOfSegments, int ratioCoef) {
        countOfSegments = numberOfSegments;
        delta = (7 - 5) / (double) countOfSegments;
        h = delta / (double) ratioCoef;
    }

    public static double getDelta() {
        return delta;
    }

    public static double getValueByIndex(int index) {
        return countValue(5 + index * h);
    }

    private static double countValue(double x) {
        return Math.exp(x) * Math.cos(x);
    }

    public static double getFirstValue() {
        return getValueByIndex(0);
    }

    public static double getEndValue() {
        return getValueByIndex(countOfSegments * (int) (delta / h));
    }
}
