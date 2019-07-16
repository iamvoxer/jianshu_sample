package d1.sample.feign;

import org.springframework.stereotype.Service;

/**
 * @author liuyi
 */
@Service
public class TestService1 {
    public String test1(){
        return "TestService1.test1()";
    }
}
