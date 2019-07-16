package d1.sample.feign;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

/**
 * @author liuyi
 */
@RestController
public class TestController {
    @Autowired
    TestService1 testService1;
    @Autowired
    TestService2 testService2;

    @RequestMapping(value = "/test", method = RequestMethod.GET)
    public String test1(String para) {
        String temp1 = testService2.get1("ttt");
        String temp2 = testService1.test1();
        String temp3 = testService2.get2(23);
        String temp4 = testService2.get4("mytoken2");
        Map<String, Object> temp5 = testService2.get3(34);
        TestModel model1 = new TestModel();
        model1.setName("testname");
        model1.setSex(23);
        String temp6 = testService2.post1(model1);
        String temp7 = testService2.post2(model1, "mytoken3");
        String temp8 = testService2.put1(model1,"mytoken4");
        String temp9 = testService2.del1(45);
        return temp1 + "\n" + temp2 + "\n" + temp3 + "\n" + temp4 + "\n" + temp5.get("name")
                + "\n" + temp5.get("sex") + "\n" + temp6 + "\n" + temp7+ "\n" + temp8+ "\n" + temp9;
    }
}
