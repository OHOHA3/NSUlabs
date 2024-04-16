package integration;

public class QuadratureFormula {
    public static double count(int countOfSegments) {
        WorkWithFunction.setParameters(countOfSegments, 6);
        double value = 0;
        for (int i = 1; i < countOfSegments; i++) {
            value += 2 * WorkWithFunction.getValueByIndex(6 * i);
            value += 3 * WorkWithFunction.getValueByIndex(6 * i + 2);
            value += 3 * WorkWithFunction.getValueByIndex(6 * i + 4);
        }
        value += 3 * WorkWithFunction.getValueByIndex(2);
        value += 3 * WorkWithFunction.getValueByIndex(4);
        value += WorkWithFunction.getFirstValue();
        value += WorkWithFunction.getEndValue();
        value = value * WorkWithFunction.getDelta() / 8;
        return value;
    }
}
