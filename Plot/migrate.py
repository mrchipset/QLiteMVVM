import re


header_list = ['global.h', 
    'vector2d.h', 
    'painter.h', 
    'paintbuffer.h', 
    'layer.h', 
    'axis/range.h', 
    'selection.h', 
    'selectionrect.h', 
    'layout.h', 
    'lineending.h', 
    'axis/axisticker.h', 
    'axis/axistickerdatetime.h', 
    'axis/axistickertime.h', 
    'axis/axistickerfixed.h', 
    'axis/axistickertext.h', 
    'axis/axistickerpi.h', 
    'axis/axistickerlog.h', 
    'axis/axis.h', 
    'scatterstyle.h', 
    'datacontainer.h', 
    'plottable.h', 
    'item.h', 
    'core.h', 
    'plottable1d.h', 
    'colorgradient.h', 
    'selectiondecorator-bracket.h', 
    'layoutelements/layoutelement-axisrect.h', 
    'layoutelements/layoutelement-legend.h', 
    'layoutelements/layoutelement-textelement.h', 
    'layoutelements/layoutelement-colorscale.h', 
    'plottables/plottable-graph.h', 
    'plottables/plottable-curve.h', 
    'plottables/plottable-bars.h', 
    'plottables/plottable-statisticalbox.h', 
    'plottables/plottable-colormap.h', 
    'plottables/plottable-financial.h', 
    'plottables/plottable-errorbar.h', 
    'items/item-straightline.h', 
    'items/item-line.h', 
    'items/item-curve.h', 
    'items/item-rect.h', 
    'items/item-text.h', 
    'items/item-ellipse.h', 
    'items/item-pixmap.h', 
    'items/item-tracer.h', 
    'items/item-bracket.h']

source_list = ['vector2d.cpp',
	'painter.cpp',
	'paintbuffer.cpp',
	'layer.cpp',
	'axis/range.cpp',
	'selection.cpp',
	'selectionrect.cpp',
	'layout.cpp',
	'lineending.cpp',
	'axis/axisticker.cpp',
	'axis/axistickerdatetime.cpp',
	'axis/axistickertime.cpp',
	'axis/axistickerfixed.cpp',
	'axis/axistickertext.cpp',
	'axis/axistickerpi.cpp',
	'axis/axistickerlog.cpp',
	'axis/axis.cpp',
	'scatterstyle.cpp',
	# 'datacontainer.cpp',
	'plottable.cpp',
	'item.cpp',
	'core.cpp',
	# 'plottable1d.cpp',
	'colorgradient.cpp',
	'selectiondecorator-bracket.cpp',
	'layoutelements/layoutelement-axisrect.cpp',
	'layoutelements/layoutelement-legend.cpp',
	'layoutelements/layoutelement-textelement.cpp',
	'layoutelements/layoutelement-colorscale.cpp',
	'plottables/plottable-graph.cpp',
	'plottables/plottable-curve.cpp',
	'plottables/plottable-bars.cpp',
	'plottables/plottable-statisticalbox.cpp',
	'plottables/plottable-colormap.cpp',
	'plottables/plottable-financial.cpp',
	'plottables/plottable-errorbar.cpp',
	'items/item-straightline.cpp',
	'items/item-line.cpp',
	'items/item-curve.cpp',
	'items/item-rect.cpp',
	'items/item-text.cpp',
	'items/item-ellipse.cpp',
	'items/item-pixmap.cpp',
	'items/item-tracer.cpp',
	'items/item-bracket.cpp']

with open('qcustomplot.h', 'w') as f:
    f.write(
    '''
    #ifndef QCUSTOMPLOT_H
    #define QCUSTOMPLOT_H
    ''')
    header_pattern = re.compile('(.*)#include(\s*)"(.*).h"(.*)')
    source_pattern = re.compile('(.*)#include(\s*)"(.*).cpp"(.*)')
    for file_name in header_list:
        path = str('src/%s' % file_name)
        fh = open(path, 'r')
        f.write('//Start %s\n' % path)
        line = fh.readlines()
        lines = []
        for l in line:
            if header_pattern.match(l) is None:
                if source_pattern.match(l) is None:
                    lines.append(l)
                else:
                    result = source_pattern.match(l)
                    sfn = result.group(3)
                    sf = open('src/%s.cpp' % sfn, 'r')
                    sfl = sf.readlines()
                    sfll = []
                    for ll in sfl:
                        if header_pattern.match(ll) is None:
                            sfll.append(ll)
                    lines.extend(sfll)
        f.writelines(lines)
        f.write('//End %s\n\n\n' % path)
    f.write(
    '''
    #endif\n
    ''')

with open('qcustomplot.cpp', 'w') as f:
    f.write(
    '''
    #include "qcustomplot.h"\n\n
    ''')
    for file_name in source_list:
        path = str('src/%s' % file_name)
        fh = open(path, 'r')
        f.write('//Start %s\n' % path)
        line = fh.readlines()
        lines = []
        for l in line:
            if header_pattern.match(l) is None:
                lines.append(l)
        f.writelines(lines)
        f.write('//End %s\n\n\n' % path)