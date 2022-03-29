function SectionWrapper({ label, children }) {
  return (
    <div className="py-6">
      <h1 className="text-3xl font-bold">{label}</h1>
      {children}
    </div>
  );
}

export default SectionWrapper;
