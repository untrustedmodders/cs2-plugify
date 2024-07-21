#pragma once

#include <functional>
#include <string>

#include <tier0/dbg.h>

#include <logger/detailed.hpp>
#include <logger/error.hpp>
#include <logger/message.hpp>
#include <logger/throw_assert.hpp>
#include <logger/warning.hpp>

#include <plugify/log.h>

namespace plugifyMM
{
	class MMLogger final : public CLoggingDetailed, public CLoggingMessage, public CLoggingWarning, public CLoggingThrowAssert, public CLoggingError, public plugify::ILogger
	{
	public:
		MMLogger(const char *pszName, RegisterTagsFunc pfnRegisterTagsFunc, int iFlags = 0, LoggingVerbosity_t eVerbosity = LV_DEFAULT, const Color &aDefault = UNSPECIFIED_LOGGING_COLOR);
		~MMLogger() override = default;

		bool IsChannelEnabled(LoggingSeverity_t eSeverity) const;
		bool IsChannelEnabled(LoggingVerbosity_t eVerbosity) const;
		LoggingVerbosity_t GetChannelVerbosity() const;
		Color GetColor() const;
		LoggingChannelFlags_t GetFlags() const;

		void Log(std::string_view message, plugify::Severity severity) override;

		void SetSeverity(plugify::Severity severity);

	protected:
		LoggingResponse_t InternalMessage(LoggingSeverity_t eSeverity, const char *pszContent) override;
		LoggingResponse_t InternalMessage(LoggingSeverity_t eSeverity, const Color &aColor, const char *pszContent) override;
		LoggingResponse_t InternalMessage(LoggingSeverity_t eSeverity, const LeafCodeInfo_t &aCode, const char *pszContent) override;
		LoggingResponse_t InternalMessage(LoggingSeverity_t eSeverity, const LeafCodeInfo_t &aCode, const Color &aColor, const char *pszContent) override;

		LoggingResponse_t InternalMessageFormat(LoggingSeverity_t eSeverity, const char *pszFormat, ...) override;
		LoggingResponse_t InternalMessageFormat(LoggingSeverity_t eSeverity, const Color &aColor, const char *pszFormat, ...) override;
		LoggingResponse_t InternalMessageFormat(LoggingSeverity_t eSeverity, const LeafCodeInfo_t &aCode, const char *pszFormat, ...) override;
		LoggingResponse_t InternalMessageFormat(LoggingSeverity_t eSeverity, const LeafCodeInfo_t &aCode, const Color &aColor, const char *pszFormat, ...) override;

	private:
		plugify::Severity m_nSeverity{ plugify::Severity::None };
		LoggingChannelID_t m_nChannelID;
	};
} // namespace plugifyMM
