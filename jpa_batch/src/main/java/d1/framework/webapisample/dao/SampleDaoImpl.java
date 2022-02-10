package d1.framework.webapisample.dao;

import d1.framework.webapisample.entity.SampleEntity;
import d1.framework.webapisample.utils.BatchExecuteRepository;

import javax.persistence.EntityManager;

/**
 * @author Buter
 */
public class SampleDaoImpl extends BatchExecuteRepository<SampleEntity, String> {

    public SampleDaoImpl(EntityManager em) {
        super(SampleEntity.class, em);
    }

}