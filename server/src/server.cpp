#include <iostream>
#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "task_factory.hpp"
#include "object.hpp"
#include "sender.hpp"

using namespace engine;
using namespace task_factory;
using namespace data;
using namespace inventory;
using namespace tasks;

class ReportSender: public Sender {
public:
    virtual void send(const Data& data) const {
        std::cout << "report: result = " << std::endl;
    }
};
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

static inline TaskFactory get_task_factory();
static Object get_create_gpio_task_data(int id, const Gpio::Direction& dir);
static Object get_set_gpio_task_data(int id, const Gpio::Value& val);
static Object get_delete_gpio_task_data(int id);


Inventory<int, Gpio> gpio_inventory;
int main(void) {
    TaskFactory mcu_task_factory(get_task_factory());
    ReportSender sender;
    Engine engine(mcu_task_factory, sender);

    int gpio_num = 12;

    engine.run_task(get_create_gpio_task_data(gpio_num, Gpio::Direction::INPUT));

    if (!gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("create task failed");
    }

    engine.run_task(get_set_gpio_task_data(gpio_num, Gpio::Value::HIGH));

    engine.run_task(get_delete_gpio_task_data(gpio_num));

    if (gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("delete task failed");
    }

    return 0;
}

// static functions/classes implementations

inline Object get_create_gpio_task_data(int id, const Gpio::Direction& dir) {
    Object data;
    data.add("task_type", String("create_gpio"));
    data.add("gpio_id", Integer(id));
    data.add("gpio_dir", Integer(static_cast<int>(dir)));
    return data;
}

static Object get_set_gpio_task_data(int id, const Gpio::Value& val) {
    Object data;
    data.add("task_type", String("set_gpio"));
    data.add("gpio_id", Integer(id));
    data.add("gpio_val", Integer(static_cast<int>(val)));
    return data;
}

inline Object get_delete_gpio_task_data(int id) {
    Object data;
    data.add("task_type", String("delete_gpio"));
    data.add("gpio_id", Integer(id));
    return data;
}

static inline TaskFactory get_task_factory() {
    TaskFactory factory(
        [](const Data& cfg)-> std::string {
            return Data::cast<String>(Data::cast<Object>(cfg).access("task_type")).get();
        }
    );

    factory.set_creator(
        "create_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    inventory_ptr->put(12, std::shared_ptr<Gpio>(new Gpio(12, Gpio::Direction::INPUT)));
                },
                cfg
            );
        }
    );

    factory.set_creator(
        "set_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    (inventory_ptr->get(12))->set(Gpio::Value::HIGH);
                },
                cfg
            );
        }
    );

    factory.set_creator(
        "delete_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    inventory_ptr->remove(12);
                },
                cfg
            );
        }
    );
    return factory;
}