include(manual.h)dnl
HEADER(resource_monitor_histograms)

SECTION(NAME)
BOLD(resource_monitor_histograms) - create HTML pages and graphs of resource monitor data

SECTION(SYNOPSIS)
CODE(BOLD(resource_monitor_histograms [options] -D monitor_data_directory output_directory [workflow_name]))
CODE(BOLD(resource_monitor_histograms [options] -L monitor_data_file_list output_directory [workflow_name]))
CODE(BOLD(resource_monitor_histograms [options] output_directory < monitor_data_file_list  [workflow_name]))

SECTION(DESCRIPTION)

BOLD(resource_monitor_histograms) is a tool to visualize resource usage as
reported by BOLD(resource_monitor). BOLD(resource_monitor_histograms) expects
its input to be a directory with resource summary files (with the -D option),
or a file listing the paths of summary files (-L option or from standard
input). Results are written to BOLD(output_directory) in the form of several
webpages showing histograms and statistics per resource.

SECTION(ARGUMENTS)

SUBSECTION(Input Options)
OPTIONS_BEGIN
OPTION_PAIR(-D, monitor_data_directory)Directory with resource summaries. All files with a .summary extension are read. Subdirectories are read recursevely.
OPTION_PAIR(-L, monitor_data_file_list)File with one summary file path per line.
OPTIONS_END

SUBSECTION(Output Options)
OPTIONS_BEGIN
OPTION_PAIR(` output_directory')The path in which to store the visualizations. See index.html for the root of the visualization.
OPTION_ITEM(` workflow_name')Optional name to include to describe the workflow being visualized.
OPTION_ITEM(` s')Generate histograms per task categories. Requires the summary files to have the field BOLD(category)
OPTION_PAIR(-f,str)Select which fields for the histograms. Each field is represented by a single character. Default is: tcvmsrwhz. The available fields are:
OPTIONS_END

LONGCODE_BEGIN
t: wall time
c: cpu time
v: virtual memory
m: resident memory
s: swap memory
r: read bytes
w: written bytes
n: num files
z: footprint
LONGCODE_END

SUBSECTION(Debugging Options)
OPTIONS_BEGIN
OPTION_TRIPLET(-d, debug, subsystem)Enable debugging for this subsystem.
OPTION_TRIPLET(-o,debug-file,file)Write debugging output to this file. By default, debugging is sent to stderr (":stderr"). You may specify logs be sent to stdout (":stdout"), to the system syslog (":syslog"), or to the systemd journal (":journal").
OPTION_ITEM(`--verbose')Display runtime progress on stdout.
OPTIONS_END


SECTION(EXAMPLES)

Most common usage:

LONGCODE_BEGIN
% resource_monitor_histograms -D my_summary_files_directory my_histograms my_workflow_name
% # open my_histograms/index.html
LONGCODE_END

Splitting on categories, generating only memory related histograms:

LONGCODE_BEGIN
% resource_monitor_histograms -s -fvms -D my_summary_files_directory my_histograms my_workflow_name
% # open my_histograms/index.html
LONGCODE_END

SECTION(COPYRIGHT)

COPYRIGHT_BOILERPLATE

FOOTER
