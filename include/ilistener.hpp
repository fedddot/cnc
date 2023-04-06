#ifndef	__ILISTENER_HPP__
#define	__ILISTENER_HPP__

namespace common
{
	template <class Event>
	class IListener
	{
	public:
		virtual ~IListener() noexcept = 0;
		virtual void onEvent(const Event& event) = 0;
	}; // IListener

	template <class Event>
	IListener<Event>::~IListener() noexcept
	{
	}

} // common

#endif // __ILISTENER_HPP__