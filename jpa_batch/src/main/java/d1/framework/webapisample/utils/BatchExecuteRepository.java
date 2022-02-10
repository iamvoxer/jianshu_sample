package d1.framework.webapisample.utils;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.data.jpa.repository.support.SimpleJpaRepository;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.EntityManager;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * 实现sql批量执行，目前只实现了批量插入，是通过重写SimpleJpaRepository的saveAll来实现
 *
 * @author maorui
 */
public class BatchExecuteRepository<T, ID> extends SimpleJpaRepository<T, ID> {

    @Value("${spring.jpa.properties.hibernate.jdbc.batch_size:500}")
    private long batchSize;
    private final EntityManager entityManager;

    public BatchExecuteRepository(Class<T> clazz, EntityManager entityManager) {
        super(clazz, entityManager);
        this.entityManager = entityManager;
    }


    @Transactional(rollbackFor = Exception.class)
    @Override
    public <S extends T> List<S> saveAll(Iterable<S> entities) {
        Iterator<S> iterator = entities.iterator();
        List<S> list = new ArrayList<>();

        int index = 0;
        while (iterator.hasNext()) {
            S next = iterator.next();
            entityManager.persist(next);

            index++;
            if (index % batchSize == 0) {
                entityManager.flush();
                entityManager.clear();
            }

            list.add(next);
        }
        if (index % batchSize != 0) {
            entityManager.flush();
            entityManager.clear();
        }

        return list;
    }

}