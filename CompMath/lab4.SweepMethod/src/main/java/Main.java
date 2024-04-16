import java.util.Arrays;
import java.util.Scanner;

public class Main {
    private static int N;
    private static double eps;
    private static double gam;
    private static double a;
    private static double b;
    private static double c;
    private static double f;

    public static void main(String[] args) {
        readData();
        double[] cColumn = fillDiagonalNumbers();
        double[] fColumn = fillRightColumn();
        double[] alphaColumn = new double[N];
        double[] betaColumn = new double[N];
        straightPass(alphaColumn, betaColumn, cColumn, fColumn);
        double[] answer = reversePass(alphaColumn, betaColumn);
        System.out.println(Arrays.toString(answer));
    }

    private static void readData() {
        Scanner scanner = new Scanner(System.in);
        System.out.print("N: ");
        N = scanner.nextInt();
        System.out.print("eps: ");
        eps = scanner.nextDouble();
        System.out.print("gam: ");
        gam = scanner.nextDouble();
        System.out.print("a: ");
        a = -scanner.nextDouble();
        System.out.print("b: ");
        b = -scanner.nextDouble();
        if (gam == 0) {
            System.out.print("c: ");
            c = scanner.nextDouble();
            System.out.print("f: ");
            f = scanner.nextDouble();
        }
    }

    private static double[] fillDiagonalNumbers() {
        double[] diagonalNumbers = new double[N];
        if (gam == 0) {
            for (int i = 0; i < N; i++) {
                diagonalNumbers[i] = c;
            }
        } else {
            for (int i = 1; i <= N; i++) {
                diagonalNumbers[i - 1] = 2 * i + gam;
            }
        }
        return diagonalNumbers;
    }

    private static double[] fillRightColumn() {
        double[] rightColumn = new double[N];
        if (gam == 0) {
            for (int i = 0; i < N; i++) {
                rightColumn[i] = f + eps;
            }
        } else {
            for (int i = 1; i <= N; i++) {
                rightColumn[i - 1] = 2 * (i + 1) + gam;
            }
        }
        return rightColumn;
    }

    private static void straightPass(double[] alphaColumn, double[] betaColumn,
                                     double[] cColumn, double[] fColumn) {
        alphaColumn[0] = b / cColumn[0];
        betaColumn[0] = fColumn[0] / cColumn[0];
        for (int i = 1; i < N; i++) {
            alphaColumn[i] = b / (cColumn[i] - alphaColumn[i - 1] * a);
            betaColumn[i] = (fColumn[i] + betaColumn[i - 1] * a) / (cColumn[i] - alphaColumn[i - 1] * a);
        }
    }

    private static double[] reversePass(double[] alphaColumn, double[] betaColumn) {
        double[] answer = new double[N];
        answer[N - 1] = betaColumn[N - 1];
        for (int i = N - 2; i >= 0; i--) {
            answer[i] = alphaColumn[i] * answer[i + 1] + betaColumn[i];
        }
        return answer;
    }
}
