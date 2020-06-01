//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_CORE_INCLUDE_DELEGATE_H
#define DIGIT_RECOGNIZER_CORE_INCLUDE_DELEGATE_H

#include <tuple>
#include <utility>
#include <functional>
#include <type_traits>
#include <cassert>

namespace DelegateImpl {
	template<typename ReturnType, typename... Args>
	auto FunctionPointer(ReturnType(*)(Args...)) -> ReturnType(*)(Args...);

	template<typename ReturnType, typename Type, typename... Args, typename Other>
	auto FunctionPointer(ReturnType(*)(Type, Args...), Other &&) -> ReturnType(*)(Args...);

	template<typename Class, typename ReturnType, typename... Args, typename... Other>
	auto FunctionPointer(ReturnType(Class::*)(Args...), Other &&...) -> ReturnType(*)(Args...);

	template<typename Class, typename ReturnType, typename... Args, typename... Other>
	auto FunctionPointer(ReturnType(Class::*)(Args...) const, Other &&...) -> ReturnType(*)(Args...);

	template<typename Class, typename Type, typename... Other>
	auto FunctionPointer(Type Class::*, Other &&...) -> Type(*)();

	template<typename... Type>
	using FunctionPointerType = decltype(FunctionPointer(std::declval<Type>()...));

	template<typename... Class, typename Ret, typename... Args>
	constexpr auto indexSequenceFor(Ret(*)(Args...))
	{
		return std::index_sequence_for < Class..., Args...>{};
	}
}

template<auto>
struct ConnectArgType {};

template<auto Function>
inline constexpr ConnectArgType<Function> ConnectArg{};

template<typename>
class Delegate;

template<typename ReturnType, typename ... Args>
class Delegate<ReturnType(Args...)> {
	template<auto Candidate, std::size_t ... Index>
	auto wrap(std::index_sequence<Index...>) noexcept
	{
		return [](const void *, Args ... args) -> ReturnType {
			const auto arguments = std::forward_as_tuple(std::forward<Args>(args) ...);
			return ReturnType(
					std::invoke(Candidate,
								std::forward<std::tuple_element_t<Index, std::tuple<Args...>>>(
										std::get<Index>(arguments)
								) ...
					)
			);
		};
	}

	template<auto Candidate, typename Type, std::size_t ... Index>
	auto wrap(Type &, std::index_sequence<Index...>) noexcept
	{
		return [](const void *payload, Args ... args) -> ReturnType {
			const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
			Type *curr = static_cast<Type *>(
					const_cast<std::conditional_t<std::is_const_v<Type>, const void *, void *>>(
							payload
					)
			);
			return ReturnType(
					std::invoke(Candidate,
								*curr,
								std::forward<std::tuple_element_t<Index, std::tuple<Args...>>>(
										std::get<Index>(arguments)
								) ...
					)
			);
		};
	}

	template<auto Candidate, typename Type, std::size_t ... Index>
	auto wrap(Type *, std::index_sequence<Index...>) noexcept
	{
		return [](const void *payload, Args ... args) -> ReturnType {
			const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
			Type *curr = static_cast<Type *>(
					const_cast<std::conditional_t<std::is_const_v<Type>, const void *, void *>>(
							payload
					)
			);
			return ReturnType(
					std::invoke(Candidate,
								curr,
								std::forward<std::tuple_element_t<Index, std::tuple<Args...>>>(
										std::get<Index>(arguments)
								) ...
					)
			);
		};
	}

public:
	using Target = ReturnType(const void *, Args...);
	using Function = ReturnType(Args...);
	using Result = ReturnType;

	Delegate() noexcept = default;

	template<auto Candidate>
	Delegate(ConnectArgType<Candidate>) noexcept : Delegate{}
	{
		connect<Candidate>();
	}

	template<auto Candidate, typename Type>
	Delegate(ConnectArgType<Candidate>, Type &&valueOrInstance) noexcept : Delegate{}
	{
		connect<Candidate>(std::forward<Type>(valueOrInstance));
	}

	Delegate(Target *function, const void *payload = nullptr) noexcept : Delegate{}
	{
		connect(function, payload);
	}

	template<auto Candidate>
	void connect() noexcept
	{
		m_data = nullptr;

		if constexpr(std::is_invocable_r_v<ReturnType, decltype(Candidate), Args...>)
			m_function = [](const void *, Args... args) -> ReturnType {
				return ReturnType(std::invoke(Candidate, std::forward<Args>(args)...));
			};
		else if constexpr(std::is_member_pointer_v<decltype(Candidate)>)
			m_function = wrap<Candidate>(
					DelegateImpl::indexSequenceFor<std::tuple_element_t<0, std::tuple<Args...>>>(
							DelegateImpl::FunctionPointerType<decltype(Candidate)>{}
					)
			);
		else
			m_function = wrap<Candidate>(
					DelegateImpl::indexSequenceFor(DelegateImpl::FunctionPointerType<decltype(Candidate)>{})
			);
	}

	template<auto Candidate, typename Type>
	void connect(Type &valueOrInstance) noexcept
	{
		m_data = &valueOrInstance;

		if constexpr(std::is_invocable_r_v<ReturnType, decltype(Candidate), Type &, Args...>)
			m_function = [](const void *payload, Args... args) -> ReturnType {
				Type *curr = static_cast<Type *>(
						const_cast<std::conditional_t<std::is_const_v<Type>, const void *, void *>>(
								payload
						)
				);
				return ReturnType(std::invoke(Candidate, *curr, std::forward<Args>(args)...));
			};
		else
			m_function = wrap<Candidate>(valueOrInstance,
										 DelegateImpl::indexSequenceFor(
												 DelegateImpl::FunctionPointerType<decltype(Candidate), Type>{}
										 )
			);
	}

	template<auto Candidate, typename Type>
	void connect(Type *valueOrInstance) noexcept
	{
		m_data = valueOrInstance;

		if constexpr(std::is_invocable_r_v<ReturnType, decltype(Candidate), Type *, Args...>)
			m_function = [](const void *payload, Args... args) -> ReturnType {
				Type *curr = static_cast<Type *>(
						const_cast<std::conditional_t<std::is_const_v<Type>, const void *, void *>>(
								payload
						)
				);
				return ReturnType(std::invoke(Candidate, curr, std::forward<Args>(args)...));
			};
		else
			m_function = wrap<Candidate>(valueOrInstance,
										 DelegateImpl::indexSequenceFor(
												 DelegateImpl::FunctionPointerType<decltype(Candidate), Type>{}
										 )
			);
	}

	void connect(Target *function, const void *payload = nullptr) noexcept
	{
		m_function = function;
		m_data = payload;
	}

	void reset() noexcept
	{
		m_function = nullptr;
		m_data = nullptr;
	}

	[[nodiscard]]
	const void *instance() const noexcept
	{
		return m_data;
	}

	ReturnType operator()(Args ... args) const
	{
		assert(m_function);
		return m_function(m_data, std::forward<Args>(args)...);
	}

	explicit operator bool() const noexcept
	{
		return m_function;
	}

	bool operator==(const Delegate<ReturnType(Args...)> &other) const noexcept
	{
		return m_function == other.m_function && m_data == other.m_data;
	}

private:
	Target *m_function = nullptr;
	const void *m_data = nullptr;
};

template<typename Ret, typename... Args>
bool operator!=(const Delegate<Ret(Args...)> &lhs, const Delegate<Ret(Args...)> &rhs) noexcept
{
	return !(lhs == rhs);
}

template<auto Candidate>
Delegate(ConnectArgType<Candidate>) noexcept -> Delegate<
		std::remove_pointer_t<DelegateImpl::FunctionPointerType<decltype(Candidate)>>
>;

template<auto Candidate, typename Type>
Delegate(ConnectArgType<Candidate>, Type &&) noexcept -> Delegate<
		std::remove_pointer_t<DelegateImpl::FunctionPointerType<decltype(Candidate), Type>>
>;

template<typename Ret, typename... Args>
Delegate(Ret(*)(const void *, Args...), const void * = nullptr) noexcept -> Delegate<Ret(Args...)>;

#endif //DIGIT_RECOGNIZER_CORE_INCLUDE_DELEGATE_H
