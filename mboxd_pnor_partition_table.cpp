#include "mboxd_pnor_partition_table.h"
#include "mbox.h"
#include "pnor_partition_table.hpp"
#include <experimental/filesystem>

struct vpnor_partition_table
{
    openpower::virtual_pnor::partition::Table* table = nullptr;
};

void vpnor_create_partition_table(struct mbox_context *context)
{
    if (context && !context->vpnor)
    {
        context->vpnor = new vpnor_partition_table;
        context->vpnor->table =
            new openpower::virtual_pnor::partition::Table;
    }
}

void vpnor_create_partition_table_from_path(struct mbox_context *context,
                                            const char *path)
{
    std::experimental::filesystem::path dir(path);

    if (context && !context->vpnor)
    {
        context->vpnor = new vpnor_partition_table;
        context->vpnor->table =
            new openpower::virtual_pnor::partition::Table(std::move(dir));
    }
}

size_t vpnor_get_partition_table_size(const struct mbox_context *context)
{
    return context && context->vpnor ?
        context->vpnor->table->size() : 0;
}

const struct pnor_partition_table* vpnor_get_partition_table(
                                       const struct mbox_context *context)
{
    return context && context->vpnor ?
        &(context->vpnor->table->getHostTable()) : nullptr;
}

const struct pnor_partition* vpnor_get_partition(
                                 const struct mbox_context *context,
                                 const size_t offset)
{
    return context && context->vpnor ?
        &(context->vpnor->table->partition(offset)) : nullptr;
}

void vpnor_destroy_partition_table(struct mbox_context *context)
{
    if(context && context->vpnor)
    {
        delete context->vpnor->table;
        delete context->vpnor;
        context->vpnor = nullptr;
    }
}