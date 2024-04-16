import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    private static double a;
    private static double b;
    private static double c;
    private static double eps;
    private static final double delta = 10;

    public static void main(String[] args) {
        readNumbers();
        ArrayList<Double> roots = findRoots();
        printRoots(roots);
    }

    static void readNumbers() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("a = ");
        a = scanner.nextDouble();
        System.out.print("b = ");
        b = scanner.nextDouble();
        System.out.print("c = ");
        c = scanner.nextDouble();
        System.out.print("eps = ");
        eps = scanner.nextDouble();
        System.out.println("x^3 + " + a + "x^2 + " + b + "x + " + c + " = 0");
        System.out.println("eps = " + eps);
    }

    static ArrayList<Double> findRoots() {
        ArrayList<Double> roots = new ArrayList<>();
        double discriminant = a * a - 3 * b;
        if (discriminant < eps) {
            if (Math.abs(funcValue(0)) < eps) {
                roots.add((double) 0);
            }
            if (funcValue(0) < -eps) {
                roots.add(positiveInfinityBisection(0));
            }
            if (funcValue(0) > eps) {
                roots.add(negativeInfinityBisection(0));
            }
        } else {
            double firstExtremum = (-a - Math.sqrt(discriminant)) / 3;
            double secondExtremum = (-a + Math.sqrt(discriminant)) / 3;
            if (funcValue(firstExtremum) > eps && funcValue(secondExtremum) < -eps) {
                roots.add(negativeInfinityBisection(firstExtremum));
                roots.add(limitedBisection(firstExtremum, secondExtremum, false));
                roots.add(positiveInfinityBisection(secondExtremum));
            }
            if (funcValue(firstExtremum) > eps && funcValue(secondExtremum) > eps) {
                roots.add(negativeInfinityBisection(firstExtremum));
            }
            if (funcValue(firstExtremum) < -eps && funcValue(secondExtremum) < -eps) {
                roots.add(positiveInfinityBisection(secondExtremum));
            }
            if (funcValue(firstExtremum) > eps && Math.abs(funcValue(secondExtremum)) < eps) {
                roots.add(negativeInfinityBisection(firstExtremum));
                roots.add(secondExtremum);
            }
            if (Math.abs(funcValue(firstExtremum)) < eps && funcValue(secondExtremum) < -eps) {
                roots.add(firstExtremum);
                roots.add(positiveInfinityBisection(secondExtremum));
            }
            if (Math.abs(funcValue(firstExtremum)) < eps && Math.abs(funcValue(secondExtremum)) < eps) {
                roots.add((firstExtremum + secondExtremum) / 2);
            }
        }
        return roots;
    }

    static double funcValue(double x) {
        return x * x * x + a * x * x + b * x + c;
    }

    static double limitedBisection(double leftNumber, double rightNumber, boolean increasing) {
        double middleNumber = (leftNumber + rightNumber) / 2;
        double funcResult = funcValue(middleNumber);
        while (Math.abs(funcResult) > eps) {
            if (funcResult < -eps) {
                if (increasing) {
                    leftNumber = middleNumber;
                } else {
                    rightNumber = middleNumber;
                }
            } else {
                if (increasing) {
                    rightNumber = middleNumber;
                } else {
                    leftNumber = middleNumber;
                }
            }
            middleNumber = (leftNumber + rightNumber) / 2;
            funcResult = funcValue(middleNumber);
        }
        return middleNumber;
    }

    static double positiveInfinityBisection(double leftNumber) {
        double rightNumber = leftNumber + delta;
        while (funcValue(rightNumber) < -eps) {
            leftNumber += delta;
            rightNumber += delta;
        }
        return limitedBisection(leftNumber, rightNumber, true);
    }

    static double negativeInfinityBisection(double rightNumber) {
        double leftNumber = rightNumber - delta;
        while (funcValue(leftNumber) > eps) {
            leftNumber -= delta;
            rightNumber -= delta;
        }
        return limitedBisection(leftNumber, rightNumber, true);
    }

    static void printRoots(ArrayList<Double> roots) {
        switch (roots.size()) {
            case 1 -> System.out.println("One equation root: x = " + roots.get(0));
            case 2 -> System.out.println("Two equation root: x1 = " + roots.get(0) + " x2 = " + roots.get(1));
            case 3 -> System.out.println("Three equation roots: x1 = " + roots.get(0) +
                    " x2 = " + roots.get(1) + " x3 = " + roots.get(2));
        }
    }
}