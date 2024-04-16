import org.junit.Before;
import org.junit.Test;

import java.io.*;
import java.util.ArrayList;
import java.util.Optional;

import static java.lang.System.lineSeparator;
import static org.junit.Assert.*;

public class MainTest {
    double eps = 0.000001;

    @Test
    public void simpleZero() {
        ByteArrayInputStream input = new ByteArrayInputStream("0 0 0 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 1;
        assertEquals(roots.size(), expectedSize);
        double expectedRoot = 0;
        assertEquals(roots.get(0), expectedRoot, eps);
    }

    @Test
    public void simplePositive() {
        ByteArrayInputStream input = new ByteArrayInputStream("9 27 -91,123456 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 1;
        assertEquals(roots.size(), expectedSize);
        double expectedRoot = 1.90657800;
        assertEquals(roots.get(0), expectedRoot, eps);
    }

    @Test
    public void simpleNegative() {
        ByteArrayInputStream input = new ByteArrayInputStream("9,78592 58,1054 91,12345 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 1;
        assertEquals(roots.size(), expectedSize);
        double expectedRoot = -2.20025105;
        assertEquals(roots.get(0), expectedRoot, eps);
    }

    @Test
    public void difficultThreeRoots() {
        ByteArrayInputStream input = new ByteArrayInputStream("39,78592 -58,1054 -91,12345 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 3;
        assertEquals(roots.size(), expectedSize);
        double expectedFirstRoot = -41.14432538;
        assertEquals(roots.get(0), expectedFirstRoot, eps);
        double expectedSecondRoot = -0.95665895;
        assertEquals(roots.get(1), expectedSecondRoot, eps);
        double expectedThirdRoot = 2.31506433;
        assertEquals(roots.get(2), expectedThirdRoot, eps);
    }

    @Test
    public void difficultNegativeRoot() {
        ByteArrayInputStream input = new ByteArrayInputStream("91 58 91,12345 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 1;
        assertEquals(roots.size(), expectedSize);
        double expectedRoot = -90.3693474;
        assertEquals(roots.get(0), expectedRoot, eps);
    }

    @Test
    public void difficultPositiveRoot() {
        ByteArrayInputStream input = new ByteArrayInputStream("91 58 -910000,12345 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 1;
        assertEquals(roots.size(), expectedSize);
        double expectedRoot = 74.0723270;
        assertEquals(roots.get(0), expectedRoot, eps);
    }
    @Test
    public void difficultSecondExtremum() {
        ByteArrayInputStream input = new ByteArrayInputStream("39,8 0 0 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 2;
        assertEquals(roots.size(), expectedSize);
        double expectedFirstRoot = -39.8;
        assertEquals(roots.get(0), expectedFirstRoot, eps);
        double expectedSecondRoot = 0;
        assertEquals(roots.get(1), expectedSecondRoot, eps);
    }
    @Test
    public void difficultFirstExtremum() {
        ByteArrayInputStream input = new ByteArrayInputStream("-39,8 0 0 0,000001".getBytes());
        System.setIn(input);
        Main.readNumbers();
        ArrayList<Double> roots = Main.findRoots();
        int expectedSize = 2;
        assertEquals(roots.size(), expectedSize);
        double expectedFirstRoot = 0;
        assertEquals(roots.get(0), expectedFirstRoot, eps);
        double expectedSecondRoot = 39.8;
        assertEquals(roots.get(1), expectedSecondRoot, eps);
    }
}