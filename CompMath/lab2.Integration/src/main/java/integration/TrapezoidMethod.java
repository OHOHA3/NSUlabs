package integration;

public class TrapezoidMethod {
    public static double count(int countOfSegments) {
        WorkWithFunction.setParameters(countOfSegments, 2);
        double value = 0;
        for (int i = 1; i < countOfSegments; i++) {
            value += 2 * WorkWithFunction.getValueByIndex(2 * i);
            value += 4 * WorkWithFunction.getValueByIndex(2 * i + 1);
        }
        value += 4 * WorkWithFunction.getValueByIndex(1);
        value += WorkWithFunction.getFirstValue();
        value += WorkWithFunction.getEndValue();
        value = value * WorkWithFunction.getDelta() / 6;
        return value;
    }
}
