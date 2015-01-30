# Include namespaces for ease of use
# require 'mscorlib'


include System
include System::Windows::Media
include System::Windows::Media::Animation
include System::Windows::Controls
include System::Windows
include System::Threading
include System::Windows::Input
include System::IO
include System::IO::IsolatedStorage
using_clr_extensions UVE_Media
include UVE_Media

audio=UVE_Audio.new("ef - the first tale","data")
audio.LoadDWave(1,"bgm\\efbgm010.ogg",0)
audio.LoadDWave(2,"voice\\ed1.ogg",nil)
audio.PlayDWave(1)
# isostore=IsolatedStorageFile.new
timer=Storyboard.new


tb=TextBlock.new
tb.Text="X:0 \nY:0"
tb.name="tb"
Phone.find_name("LayoutRoot").children.add(tb)
button = Button.new
button.name="button"
button.content = "Begin"
button.width=100
Phone.find_name("LayoutRoot").children.add(button)
button.click do |s,e|
  audio.PlayDWave(1)
  if button.Content=="Begin"
    MessageBox.show("Storyboard timer begin!")
    timer.Begin
    button.Content="Stop"
  else
    MessageBox.show("Storyboard timer Stop!")
    timer.Stop
    button.Content="Begin"
  end
end

# Set the titles
# Phone.find_name("ApplicationTitle").text = "uvengine is running!"
# Phone.find_name("PageTitle").text = "IronRuby for UVEngine"

# Create a new text block
textBlock = TextBlock.new
textBlock.name = "textBlock"
textBlock.text = "IronRuby is running on Windows Phone 7!"
textBlock.foreground = SolidColorBrush.new(Colors.Green)
textBlock.font_size = 48
textBlock.text_wrapping = System::Windows::TextWrapping.Wrap
# Add the text block to the page
# Phone.find_name("ContentPanel").children.add(textBlock)


# Phone.find_name("LayoutRoot").manipulationdelta do |s,e|
#   delta_x=e.DeltaManipulation.Translation.X
#   delta_y=e.DeltaManipulation.Translation.Y
#   MessageBox.Show(Convert.ToString(delta_x)+'\n'+Convert.ToString(delta_y))
# end

# timer.begin
# Touch.FrameReported.add(method(:touch))
Touch.FrameReported do |s,e|
  point = e.GetPrimaryTouchPoint(nil)
  x = point.Position.X
  y = point.Position.Y
  Phone.find_name("button").margin = Thickness.new(x-50,y-25,430-x,0)
  Phone.find_name("tb").text = "X:"+Convert.ToString(x)+"\nY:"+Convert.ToString(y)
end



anim = DoubleAnimation.new
anim.From = 0.0
anim.To = 1.0
anim.Duration = Duration.new(TimeSpan.FromMilliseconds(500))
Storyboard.SetTarget(anim,button)
timer.Children.Add(anim)
timer.AutoReverse = true
timer.RepeatBehavior = RepeatBehavior.Forever
Storyboard.SetTargetProperty(anim, Phone.GetPropertyPath("(UIElement.Opacity)"))


