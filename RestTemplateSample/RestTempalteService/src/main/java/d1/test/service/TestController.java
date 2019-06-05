package d1.test.service;

import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.Hashtable;
import java.util.Map;

@RestController
@RequestMapping("/test")
public class TestController {
    @RequestMapping(value = "/get1", method = RequestMethod.GET)
    public String get1(String para) {
        return "get1:" + para;
    }

    @RequestMapping(value = "/get2/{id}", method = RequestMethod.GET)
    public String get2(@PathVariable Integer id) {
        return "get2:" + id;
    }

    @RequestMapping(value = "/get3/{id}", method = RequestMethod.GET)
    public Map<String, Object> get3(@PathVariable Integer id) {
        Map<String, Object> result = new Hashtable<String, Object>();
        result.put("name", "myname");
        result.put("sex", id);
        return result;
    }

    @RequestMapping(value = "/get4", method = RequestMethod.GET)
    public String get4(HttpServletRequest request) {
        return "get4:" + request.getHeader("token");
    }

    @RequestMapping(value = "/post1", method = RequestMethod.POST)
    public String post1(@RequestBody TestModel model) {
        return "post1:" + model.toString();
    }

    @RequestMapping(value = "/post2", method = RequestMethod.POST)
    public String post2(@RequestBody TestModel model, HttpServletRequest request) {
        return "post2:" + model.toString() + request.getHeader("token");
    }

    @RequestMapping(value = "/put1", method = RequestMethod.PUT)
    public String put1(@RequestBody TestModel model, HttpServletRequest request) {
        return "put1:" + model.toString() + request.getHeader("token");
    }

    @RequestMapping(value = "/del1/{id}", method = RequestMethod.DELETE)
    public String del1(@PathVariable Integer id) {
        return "del1:" + id;
    }
}
