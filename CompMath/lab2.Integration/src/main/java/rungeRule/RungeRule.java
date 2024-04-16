package rungeRule;

import integration.ParabolaMethod;
import integration.QuadratureFormula;
import integration.TrapezoidMethod;

public class RungeRule {
    public static double forParabola(int countOfSegments) {
        double s1 = ParabolaMethod.count(countOfSegments);
        double s2 = ParabolaMethod.count(countOfSegments * 2);
        double s3 = ParabolaMethod.count(countOfSegments * 4);
        double s = Math.abs((s1 - s2) / (s2 - s3));
        return Math.log(s) / Math.log(2);
    }

    public static double forQuadrature(int countOfSegments) {
        double s1 = QuadratureFormula.count(countOfSegments);
        double s2 = QuadratureFormula.count(countOfSegments * 2);
        double s3 = QuadratureFormula.count(countOfSegments * 4);
        double s = Math.abs((s1 - s2) / (s2 - s3));
        return Math.log(s) / Math.log(2);
    }

    public static double forTrapezoid(int countOfSegments) {
        double s1 = TrapezoidMethod.count(countOfSegments);
        double s2 = TrapezoidMethod.count(countOfSegments * 2);
        double s3 = TrapezoidMethod.count(countOfSegments * 4);
        double s = Math.abs((s1 - s2) / (s2 - s3));
        return Math.log(s) / Math.log(2);
    }
}
