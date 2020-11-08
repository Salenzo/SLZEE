#coding:utf-8

$glyph_widths = [5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 6, 6, 7, 6, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 3, 5, 6, 6, 6, 6, 6, 6, 6, 7, 8, 8, 8, 8, 7, 5, 6, 6, 6, 5, 5, 5, 5, 6, 7, 3, 3, 6, 3, 7, 7, 6, 7, 7, 6, 5, 5, 7, 7, 7, 6, 6, 6, 7, 6, 7, 4, 5, 6, 8, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 2, 6, 0, 3] + [nil] * 914 + [5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 4, 7, 5, 5, 5, 5, 5, 5, 7, 6, 7, 7, 6, 7, 4, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 3, 5, 6, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 6, 5, 4, 5, 4, 5, 4, 5, 4, 4, 5, 6, 3, 3, 6, 3, 7, 6, 4, 5, 5, 5, 3, 3, 6, 5, 7, 4, 4, 4, 5, 4, 6, 3, 4, 5, 7, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 2, 5, 0, 0]
$glyph_heights = [6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 3, 6, 3, 6, 3, 6, 3, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 6, 6, 2, 3, 3, 4, 4, 5, 3, 10, 8, 8, 8, 8, 10, 10, 3, 3, 6, 3, 6, 3, 6, 3, 6, 5, 5, 6, 6, 3, 3, 3, 3, 3, 3, 3, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 5, 5, 6, 6, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 0, 0] + [nil] * 914 + [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 4, 2, 3, 3, 3, 3, 4, 3, 4, 4, 4, 4, 4, 4, 4, 2, 3, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0]
$glyph_depths = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 6, 3, 6, 4, 3, 3, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 2, 2, 2, 2, 6, 6, 1, 2, 2, 4, 4, 4, 2, 10, 8, 8, 8, 8, 10, 10, 3, 3, 3, 3, 3, 3, 3, 6, 3, 3, 6, 3, 3, 3, 3, 3, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 6, 3, 3, 6, 3, 1, 1, 1, 3, 3, 3, 3, 1, 3, 5, 5, 3, 0, 3] + [nil] * 914 + [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 2, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 4, 4, 1, 2, 2, 4, 4, 3, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 4, 2, 2, 4, 2, 1, 1, 1, 3, 3, 3, 3, 1, 2, 4, 4, 2, 0, 0]
File.open("glyphs.htm", "wb") do |f|
  f.puts "<style>span { display: inline-block; position: absolute; box-shadow: 0 0 0 0.5px rgba(135,206,235,0.5) inset; }</style><img src=\"a.png\" style=\"position: absolute; left: 0; top: 0; image-rendering: pixelated;\">"
  (0...2048).each do |i|
    next if $glyph_widths[i].nil?
    f.puts "<span style=\"left: #{(i % 1024) * 8}px; top: #{(i < 1024 ? 10 : 24) - $glyph_heights[i]}px; width: #{$glyph_widths[i]}px; height: #{$glyph_heights[i] + $glyph_depths[i]}px\"></span>"
  end
end

def whd(x)
  if x.is_a?(Integer)
    if x >= 0
      return [$glyph_widths[x], $glyph_heights[x], $glyph_depths[x]]
    else
      return [-x, 0, 1]
    end
  end
  maxx = -1
  maxy = -9999
  miny = 9999
  x.each do |(ch, x, y)|
    sz = whd(ch)
    w1 = x + sz[0]
    maxx = w1 if w1 > maxx
    h1 = y - sz[1]
    miny = h1 if h1 < miny
    d1 = y + sz[2]
    maxy = d1 if d1 > maxy
  end
  [maxx, -miny, maxy]
end

def find(s, start_pos, mid_str, end_str)
  t = 0
  eof = nil
  sep = []
  start_str = s[start_pos]
  for i in start_pos...s.length
    t += 1 if s[i] == start_str
    t -= 1 if s[i] == end_str
    sep << i if t == 1 and s[i] == mid_str
    if t == 0
      eof = i
      break
    end
  end
  sep.unshift(eof)
end

def render_matrix(a)
  sz = a.map { |x| x.map { |x| whd(x) } }
  w = sz.transpose.map { |x| x.max { |a, b| a[0] <=> b[0] }[0] }
  r = []
  x = y = 0
  a.each_with_index do |ln, i|
    y += sz[i].max { |a, b| a[1] <=> b[1] }[1]
    ln.each_index do |j|
      r << [ln[j], x + (w[j] - sz[i][j][0]) / 2, y]
      x += w[j] + 13
    end
    x = 0
    y += sz[i].max { |a, b| a[2] <=> b[2] }[2] + 3
  end
  [[r, 0, -whd(r).drop(1).sum / 2]]
end

def render(s, size)
  if s.include?("\n") or s.include?("\\")
    y = 0
    return s.split(/[\n\\]/).map do |s|
      r = render(s, size)
      sz = whd(r)
      y += sz[1]
      r = [r, 0, y]
      y += sz[2] + 1
      r
    end
  end
  chars = []
  x = 0
  i = 0
  while i < s.length
    case s[i]
    when "分"
      eof, sep = find(s, i, "于", "合")
      n = render(s[(i + 1)...sep], size)
      d = render(s[(sep + 1)...eof], size)
      nsz = whd(n)
      dsz = whd(d)
      w = [nsz[0], dsz[0]].max
      chars << [n, x + 1 + (w - nsz[0]) / 2, -nsz[2] - 2]
      chars << [d, x + 1 + (w - dsz[0]) / 2, dsz[1] + 1]
      chars << [-(w + 2), x, -1]
      x += w + 3
      i = eof
    when " "
      x += 2
    when "上"
      eof, sep = find(s, i, "下", "中")
      u = render(s[(i + 1)...sep], 1)
      d = render(s[(sep + 1)...eof], 1)
      usz = whd(u)
      dsz = whd(d)
      w = [usz[0], dsz[0]].max
      psz = whd([chars.last])
      chars << [u, x, -psz[1] - usz[2] + 3]
      chars << [d, x, psz[2] + dsz[1] - 3]
      x += w + 1
      i = eof
    when "张"
      eof, _ = find(s, i, nil, "外")
      n = "零一二三四五六七八九".index(s[i + 1])
      e = s[(i + 2)...eof].split("一").map { |s| render(s, size) }
      a = []
      a << e.shift(n) until e.empty?
      chars << [render_matrix(a), x, 0]
      x += whd(chars.last.first)[0] + 1
      i = eof
    when "(", "[", "{", "左", "√"
      eof, _ = find(s, i, nil, {"(" => ")", "[" => "]", "{" => "}", "左" => "右", "√" => "出"}[s[i]])
      inner = render(s[(i + 1)...eof], size)
      sz = whd(inner)
      sz[1] += 1
      sz[2] += 1
      top = {"(" => 99, ")" => 101, "[" => 103, "]" => 103, "{" => 98, "}" => 96, "左" => 108, "右" => 108, "√" => 108, "出" => 108}
      bot = {"(" => 100, ")" => 102, "[" => 103, "]" => 103, "{" => 98, "}" => 96, "左" => 108, "右" => 108, "√" => 109, "出" => 108}
      mid = {"{" => 96, "}" => 98}
      rep = {"(" => 96, ")" => 98, "[" => 96, "]" => 98, "{" => 97, "}" => 97, "左" => 97, "右" => 97, "√" => 98, "出" => 108}
      chars << [top[s[i]], x, -sz[1]]
      chars << [mid[s[i]], x, -$glyph_depths[mid[s[i]]]] if mid[s[i]]
      ys = ((-sz[1] + $glyph_depths[top[s[i]]])...(sz[2] - $glyph_depths[bot[s[i]]])).to_a
      ys -= ((-$glyph_depths[mid[s[i]]])...0).to_a if mid[s[i]]
      for yy in ys
        chars << [rep[s[i]], x, yy]
      end
      chars << [bot[s[i]], x, sz[2] - $glyph_depths[bot[s[i]]]]
      x += 4
      chars << [inner, x, 0]
      chars << [-(sz[0] + 3), x - 2, -sz[1] - 1] if s[i] == "√"
      x += sz[0] + 1
      chars << [mid[s[eof]], x, -$glyph_depths[mid[s[eof]]]] if mid[s[eof]]
      ys = ((-sz[1] + $glyph_depths[top[s[eof]]])...(sz[2] - $glyph_depths[bot[s[eof]]])).to_a
      ys -= ((-$glyph_depths[mid[s[eof]]])...0).to_a if mid[s[eof]]
      for yy in ys
        chars << [rep[s[eof]], x, yy]
      end
      chars << [[[top[s[eof]], 0, -sz[1]], [bot[s[eof]], 0, sz[2] - $glyph_depths[bot[s[eof]]]]], x, 0]
      x += 4
      i = eof
    else
      ch = "0123456789abcdefgxyzuvwABCDΔ/()[]{}地板天花+-×÷*|N=<>≤≥不恒∫SP交并∮重πａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ∇∂RijkΣφθ        .,;r  ".index(s[i])
      if ch
        ch += size * 1024
        chars << [ch, x, 0]
        x += $glyph_widths[ch] + 1
      else
        puts "Warning: unknown character '#{s[i]}'"
      end
    end
    i += 1
  end
  chars
end
$chars = render(File.read("a.txt"), 0)

File.open("b.htm", "wb") do |f|
  f.puts "<style>span { display: inline-block; position: absolute; background-image: url(a.png); background-repeat: no-repeat; width: 8px; height: 20px; image-rendering: pixelated; } div { display: inline-block; position: absolute; background-color: black; height: 1px; }</style>"
  $chars.each do |(ch, x, y)|
    if ch.is_a?(Array)
      $chars.concat(ch.map { |(ch, xx, yy)| [ch, x + xx, y + yy] })
    elsif ch < 0
      f.puts "<div style=\"left: #{x}px; top: #{y}px; width: #{-ch}px;\"></div>"
    else
      f.puts "<span style=\"background-position: #{-(ch % 1024) * 8}px #{-(ch / 1024) * 20}px; left: #{x}px; top: #{y - (ch < 1024 ? 10 : 4)}px;\"></span>"
    end
  end
end
