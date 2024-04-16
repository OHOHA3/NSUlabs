package integration;

public class ParabolaMethod {
    public static double count(int countOfSegments) {
        WorkWithFunction.setParameters(countOfSegments, 1);
        double value = 0;
        for (int i = 1; i < countOfSegments; i++) {
            value += 2 * WorkWithFunction.getValueByIndex(i);
        }
        value += WorkWithFunction.getFirstValue();
        value += WorkWithFunction.getEndValue();
        value = value * WorkWithFunction.getDelta() / 2;
        return value;
    }
}
