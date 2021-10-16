import ida_pro
import ida_auto
import ida_loader
import ida_hexrays

ida_path = ida_loader.get_path(ida_loader.PATH_TYPE_IDB)
c_path = "%s.c" % ida_path

ida_auto.auto_wait()
ida_hexrays.decompile_many (
    c_path,
    None,
    ida_hexrays.VDRUN_NEWFILE|
    ida_hexrays.VDRUN_SILENT|
    ida_hexrays.VDRUN_MAYSTOP)

ida_pro.qexit(0)
