#include <type_traits>
#include <functional>
#include <vector>
namespace EcoSim
{
    template <class T>
    class EventCaller;

    template <class T>
    class Event;

    /// <summary>
    /// 事件调用器。可以派发它绑定的事件。通过调用`Event`类的构造函数来创建一个事件并绑定事件调用器。
    /// </summary>
    /// <typeparam name="T">回调函数的类型，必须为函数类型。例如`int(int)`。</typeparam>
    template <typename TR, typename... TArgs>
    class EventCaller<TR(TArgs...)>
    {
        friend class Event<TR(TArgs...)>;

    private:
        /// <summary>
        /// 绑定的事件
        /// </summary>
        Event<TR(TArgs...)>* target;

    public:

        /// <summary>
        /// 构造一个未绑定的调用器对象。
        /// </summary>
        EventCaller() : target(nullptr) {}

        /// <summary>
        /// 返回是否与事件绑定。
        /// </summary>
        /// <returns></returns>
        auto IsBound() -> bool { return target != nullptr; }


        /// <summary>
        /// 派发事件。
        /// </summary>
        /// <typeparam name="...Ts"></typeparam>
        /// <param name="...args"></param>
        /// <returns></returns>

        auto Dispatch(TArgs... args) -> void
        {
            for (auto&& cb : target->callbacks)
            {
                cb(args...);
            }
        }
    };

    /// <summary>
    /// 事件。用户可以直接订阅事件，但必须通过`EventCaller`派发事件。
    /// </summary>
    /// <typeparam name="T"> 回调函数的类型，必须为函数类型。例如`int(int)`</typeparam>
    template <typename TR, typename... TArgs>
    class Event<TR(TArgs...)>
    {
        friend class EventCaller<TR(TArgs...)>;

        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

    private:
        std::vector < std::function < TR(TArgs...) > > callbacks;

    public:

        /// <summary>
        /// 构造事件，并与`EventCaller`绑定。
        /// </summary>
        /// <param name="caller"></param>
        Event(EventCaller<TR(TArgs...)>& caller)
        {
            caller.target = this;
        }

        /// <summary>
        /// 以函数订阅此事件。 
        /// </summary>
        /// <param name="callback"></param>
        /// <returns></returns>
        auto Subscribe(std::function<TR(TArgs...)> callback) -> void
        {
            callbacks.push_back(callback);
        }
    };
}