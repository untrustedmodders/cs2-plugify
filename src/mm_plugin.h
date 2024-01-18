/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Sample Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This sample plugin is public domain.
 */

#pragma once

#include <ISmmPlugin.h>

namespace wizard {
	class IWizard;
}

namespace wizardMM {
	class WizardMMPlugin : public ISmmPlugin {
	public:
		bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) override;
		bool Unload(char* error, size_t maxlen) override;
		bool Pause(char* error, size_t maxlen) override;
		bool Unpause(char* error, size_t maxlen) override;
		void AllPluginsLoaded() override;

	public:
		const char* GetAuthor() override;
		const char* GetName() override;
		const char* GetDescription() override;
		const char* GetURL() override;
		const char* GetLicense() override;
		const char* GetVersion() override;
		const char* GetDate() override;
		const char* GetLogTag() override;

		std::shared_ptr<wizard::IWizard> _context;
	};

	extern WizardMMPlugin g_Plugin;

	PLUGIN_GLOBALVARS();
}