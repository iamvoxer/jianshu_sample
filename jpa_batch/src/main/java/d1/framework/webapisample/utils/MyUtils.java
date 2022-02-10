package d1.framework.webapisample.utils;

import java.util.UUID;

/**
 * @author Buter
 * @date 2020/3/20 15:16
 */
public class MyUtils {

    public static String generate32Id() {
        return UUID.randomUUID().toString().replace("-", "").toLowerCase();
    }
}
