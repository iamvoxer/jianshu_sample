package d1.framework.webapisample.service;

import d1.framework.webapisample.dao.SampleDao;
import d1.framework.webapisample.entity.SampleEntity;
import d1.framework.webapisample.utils.MyUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * 针对webadmin的登录等操作
 *
 * @author Buter
 * @date 2020/3/16 6:50
 */
@Service
public class SampleService {
    private final SampleDao sampleDao;
    private final int length = 2;
    public SampleService(SampleDao sampleDao) {
        this.sampleDao = sampleDao;
    }


    @Transactional(rollbackFor = Exception.class)
    public void insert1() throws Exception {
        insert2();
    }

    public void insert2() throws Exception {
        for (int i = 0; i < length; i++) {
            SampleEntity entity = new SampleEntity();
            entity.setId(MyUtils.generate32Id());
            entity.setIntField(i);
            entity.setStringField(i + "");
            entity.setDateTimeField(LocalDateTime.now());
            sampleDao.save(entity);
        }
    }

    @Transactional(rollbackFor = Exception.class)
    public void insert3() throws Exception {
        insert4();
    }

    public void insert4() throws Exception {
        List<SampleEntity> list = new ArrayList<>(length);
        for (int i = 0; i < length; i++) {
            SampleEntity entity = new SampleEntity();
            entity.setId(MyUtils.generate32Id());
            entity.setIntField(i);
            entity.setStringField(i + "");
            entity.setDateTimeField(LocalDateTime.now());
            list.add(entity);
        }
        sampleDao.saveAll(list);
    }
    public void clear(){
        sampleDao.deleteAll();
    }
}
