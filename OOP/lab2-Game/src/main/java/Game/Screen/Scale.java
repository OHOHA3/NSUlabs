package Game.Screen;

public class Scale {
    private static Scale instance;
    double scale;
    final double startHeight = 720;
    private int scaleFlag = 0;

    private Scale() {
    }

    public double getScale() {
        return scale;
    }

    public static Scale getInstance() {
        if (instance == null) {
            instance = new Scale();
        }
        return instance;
    }

    public void setSkale(double height) {
        if (scaleFlag == 0) {
            scale = height / startHeight;
            System.out.println(height);
            System.out.println(scale);
            scaleFlag = 1;
        }
    }

}
