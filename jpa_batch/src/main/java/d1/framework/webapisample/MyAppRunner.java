package d1.framework.webapisample;

import d1.framework.webapisample.service.SampleService;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.stereotype.Component;

/**
 * @author Buter
 * @date 2020/3/20 14:59
 */
@Component
public class MyAppRunner implements ApplicationRunner {

    private final SampleService service;

    public MyAppRunner(SampleService service) {
        this.service = service;
    }

    @Override
    public void run(ApplicationArguments args) {

        long start = System.currentTimeMillis();
        try {
            this.service.insert1();
        } catch (Exception e) {
            e.printStackTrace();
        }
        long end = System.currentTimeMillis();
        System.out.println("循环执行save（带事务）耗时(毫秒)=" + (end - start));
        this.service.clear();

        start = System.currentTimeMillis();
        try {
            this.service.insert2();
        } catch (Exception e) {
            e.printStackTrace();
        }
        end = System.currentTimeMillis();
        System.out.println("循环执行save（不带事务）耗时(毫秒)=" + (end - start));
        this.service.clear();

        start = System.currentTimeMillis();
        try {
            this.service.insert3();
        } catch (Exception e) {
            e.printStackTrace();
        }
        end = System.currentTimeMillis();
        System.out.println("执行saveAll（带事务）耗时(毫秒)=" + (end - start));
        this.service.clear();

        start = System.currentTimeMillis();
        try {
            this.service.insert4();
        } catch (Exception e) {
            e.printStackTrace();
        }
        end = System.currentTimeMillis();
        System.out.println("执行saveAll（不带事务）耗时(毫秒)=" + (end - start));
        this.service.clear();
    }
}
