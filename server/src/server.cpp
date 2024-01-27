#include "creator.hpp"
#include "data.hpp"
#include "delete_inventory_item_task.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "task.hpp"
#include "task_factory.hpp"
#include "create_inventory_item_task.hpp"
#include <iostream>
#include <map>

using namespace cnc_engine;
using namespace task_factory;
using namespace data;
using namespace basics;
using namespace inventory;
using namespace tasks;

class Gpio {
public:
    enum class Direction: int {
        INPUT,
        OUTPUT
    };
    enum class Value: int {
        LOW,
        HIGH
    };
    Gpio(int id, const Direction& direction): m_id(id), m_direction(direction), m_val(Value::LOW) {

    }
    Value get() const {
        return m_val;
    }
    void set(const Value& val) {
        m_val = val;
    }
private:
    int m_id;
    Direction m_direction;
    Value m_val;
};

class GpioCreator: public Creator<Gpio *, data::Data> {
public:
    virtual Gpio *create(const data::Data& cfg) const {
        const Object& cfg_data = Data::cast<Object>(cfg);
        Gpio::Direction gpio_dir = static_cast<Gpio::Direction>(Data::cast<Integer>(cfg_data.access("direction")).get());
        int gpio_id = Data::cast<Integer>(cfg_data.access("id")).get();
        return new Gpio(m_gpio_id, m_gpio_dir);
    }
private:
    int m_gpio_id;
    Gpio::Direction m_gpio_dir;
};

static GpioCreator s_gpio_creator;

class CreateGpioTaskCreator: public Creator<Task *, Data> {
public:
    CreateGpioTaskCreator(Inventory<int, Gpio>& gpio_inventory): m_gpio_inventory(gpio_inventory) {

    }
    virtual Task *create(const Data& data) const {
        int gpio_id = Data::cast<Integer>(Data::cast<Object>(data).access("id")).get();

        return new CreateInventoryItemTask<int, Gpio>(m_gpio_inventory, gpio_id, data, s_gpio_creator);
    }
private:
    Inventory<int, Gpio>& m_gpio_inventory;
};

class DeleteGpioTaskCreator: public Creator<Task *, Data> {
public:
    DeleteGpioTaskCreator(Inventory<int, Gpio>& gpio_inventory): m_gpio_inventory(gpio_inventory) {

    }
    virtual Task *create(const Data& data) const {
        int gpio_id = Data::cast<Integer>(Data::cast<Object>(data).access("id")).get();
        return new DeleteInventoryItemTask<int, Gpio>(m_gpio_inventory, gpio_id);
    }
private:
    Inventory<int, Gpio>& m_gpio_inventory;
};

class ReportSender: public Engine::ReportSender {
public:
    virtual void send(const Report& data) const {
        std::cout << "report: result = " << static_cast<int>(data.result()) << std::endl;
    }
};

int main(void) {
    Inventory<int, Gpio> gpio_inventory;
    CreateGpioTaskCreator create_item_task_creator(gpio_inventory);
    DeleteGpioTaskCreator delete_item_task_creator(gpio_inventory);
    
    TaskFactory task_factory;

    ReportSender sender;
    Engine engine(task_factory, sender);

    Object taskCfg;
    taskCfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::CREATE_INVENTORY_ITEM)));
    taskCfg.add("id", Integer(0));
    taskCfg.add("direction", Integer(static_cast<int>(Gpio::Direction::INPUT)));


    engine.run_task(taskCfg);

    return 0;
}