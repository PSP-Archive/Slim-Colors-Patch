#include <pspsdk.h>
#include <pspkernel.h>
#include <systemctrl.h>
#include <string.h>

PSP_MODULE_INFO("slim_colors", 0x1007, 0, 1);
STMOD_HANDLER previous;

int OnModuleStart(SceModule2 *mod)
{
	if (strcmp(mod->modname, "sysconf_plugin_module") == 0)
	{
		u32 text_end = mod->text_addr+mod->text_size;
		u32 text_addr = mod->text_addr;
		
		for (; text_addr < text_end; text_addr += 4)
		{
			if (_lw(text_addr) == 0x24040018 && _lw(text_addr+4) == 0x26530008)
			{
				text_addr -= 0x18;
				break;
			}
		}
		
		_sw(_lw(text_addr+4), text_addr);
		_sw(0x24020001, text_addr+4);
		
		sceKernelDcacheWritebackAll();
		sceKernelIcacheClearAll();
	}

	if (!previous)
		return 0;
		
	return previous(mod);
}

int module_start(SceSize args, void *argp)
{
	previous = sctrlHENSetStartModuleHandler(OnModuleStart);
	return 0;
}