import integration.ParabolaMethod;
import integration.QuadratureFormula;
import integration.TrapezoidMethod;
import rungeRule.RungeRule;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int countOfSegments = scanner.nextInt();
        System.out.println("Parabola method:");
        System.out.println("Value: " + ParabolaMethod.count(countOfSegments));
        System.out.println("Accuracy: " + RungeRule.forParabola(countOfSegments));
        System.out.println("Quadrature formula:");
        System.out.println("Value: " + QuadratureFormula.count(countOfSegments));
        System.out.println("Accuracy: " + RungeRule.forQuadrature(countOfSegments));
        System.out.println("Trapezoid method:");
        System.out.println("Value: " + TrapezoidMethod.count(countOfSegments));
        System.out.println("Accuracy: " + RungeRule.forTrapezoid(countOfSegments));
    }
}
