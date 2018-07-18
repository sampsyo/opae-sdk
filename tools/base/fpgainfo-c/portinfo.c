// Copyright(c) 2018, Intel Corporation
//
// Redistribution  and  use  in source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of  source code  must retain the  above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name  of Intel Corporation  nor the names of its contributors
//   may be used to  endorse or promote  products derived  from this  software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
// LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
// CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
// SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
// INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
// CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "fpgainfo.h"
#include "portinfo.h"
#include <opae/fpga.h>
//#include <uuid/uuid.h>

static void print_port_info(fpga_properties props)
{
	char guid_str[38];
	fpga_guid guid;
	fpga_result res = FPGA_OK;

	fpgainfo_print_common("//****** PORT ******//", props);

	res = fpgaPropertiesGetGUID(props, &guid);
	fpgainfo_print_err("reading guid from properties", res);
	uuid_unparse(guid, guid_str);

	printf("%-24s : %s\n", "Accelerator GUID", guid_str);
}

fpga_result port_filter(fpga_properties *filter, int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	fpga_result res = FPGA_OK;
	res = fpgaPropertiesSetObjectType(*filter, FPGA_ACCELERATOR);
	fpgainfo_print_err("setting type to FPGA_ACCELERATOR", res);
	return res;
}

fpga_result port_command(fpga_token *tokens, int num_tokens, int argc,
			 char *argv[])
{
	(void)argc;
	(void)argv;

	fpga_result res = FPGA_OK;
	fpga_properties props;

	int i = 0;
	for (i = 0; i < num_tokens; ++i) {
		res = fpgaGetProperties(tokens[i], &props);
		ON_FPGAINFO_ERR_GOTO(res, out_destroy,
				     "reading properties from token");
		print_port_info(props);
		fpgaDestroyProperties(&props);
	}

	return res;

out_destroy:
	fpgaDestroyProperties(&props);
	return res;
}